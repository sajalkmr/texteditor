#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LEN 100
#define MAX_LINE_LEN 1000

void print_menu()
{
    printf("\n");
    printf("1. Open File\n");
    printf("2. Save File\n");
    printf("3. Find and Replace\n");
    printf("4. Quit\n");
}

char *read_line(FILE *file)
{
    char *line = malloc(MAX_LINE_LEN);
    if (!line)
    {
        perror("Error allocating memory for line buffer");
        exit(1);
    }

    if (!fgets(line, MAX_LINE_LEN, file))
    {
        free(line);
        return NULL;
    }

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
    {
        line[len - 1] = '\0';
    }

    return line;
}

void write_line(FILE *file, const char *line)
{
    fputs(line, file);
}

void open_file(char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        perror("Error opening file for reading");
        return;
    }

    char *line;
    while ((line = read_line(file)))
    {
        printf("%s\n", line);
        free(line);
    }

    fclose(file);
}

void save_file(char *filename)
{
    FILE *file = fopen(filename, "w");

    if (!file)
    {
        perror("Error opening file for writing");
        return;
    }

    char line[MAX_LINE_LEN + 1];
    while (fgets(line, sizeof(line), stdin))
    {
        write_line(file, line);
    }

    fclose(file);

    printf("File saved successfully!\n");
}

void find_and_replace(char *filename)
{
    FILE *file = fopen(filename, "r+");

    if (!file)
    {
        perror("Error opening file for reading and writing");
        return;
    }

    char find[MAX_LINE_LEN + 1];
    char replace[MAX_LINE_LEN + 1];

    printf("Enter the string to find: ");
    fgets(find, sizeof(find), stdin);

    size_t len_find = strlen(find);

    // remove newline character from input
    if (find[len_find - 1] == '\n')
    {
        find[len_find - 1] = '\0';
        len_find--;
    }

    printf("Enter the replacement string: ");
    fgets(replace, sizeof(replace), stdin);

    size_t len_replace = strlen(replace);

    // remove newline character from input
    if (replace[len_replace - 1] == '\n')
    {
        replace[len_replace - 1] = '\0';
        len_replace--;
    }

    char *line;
    long offset;

    while ((offset = ftell(file)), (line = read_line(file)))
    {

        // check if the current line contains the search string
        char *found_at;

        while ((found_at = strstr(line, find)))
        {

            // calculate the new length of the line after replacement
            size_t new_len =
                strlen(line) - len_find + len_replace;

            // allocate memory for the new version of the line
            char *new_line = malloc(new_len + 1);

            if (!new_line)
            {

                // error allocating memory; just skip this replacement
                break;
            }

            // copy everything before the search string into the new version of the line
            strncpy(new_line, line, found_at - line);

            // copy over the replacement string into the new version of the line
            strncpy(new_line + (found_at - line), replace,
                    strlen(replace));

            // copy everything after the search string into the new version of the line
            strcpy(new_line + (found_at - line) + len_replace,
                   found_at + len_find);

            free(line);
            fseek(file, offset, SEEK_SET);
            write_line(file, new_line);
            fseek(file, strlen(new_line), SEEK_CUR);

            free(new_line);

            offset = ftell(file);
            free(read_line(file));
            fseek(file, offset - strlen(found_at) - len_find + strlen(replace), SEEK_SET);

            break;
        }
    }
    fclose(file);
}

int main()
{

    int choice;
    char filename[MAX_FILENAME_LEN];

    while (1)
    {
        print_menu();

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("\nEnter filename: ");
            scanf("%s", filename);
            open_file(filename);
            break;

        case 2:
            printf("\nEnter filename: ");
            scanf("%s", filename);
            save_file(filename);
            break;

        case 3:
            printf("\nEnter filename: ");
            scanf("%s", filename);
            find_and_replace(filename);
            break;

        case 4:
            return EXIT_SUCCESS;

        default:
            printf("\nInvalid choice!\n");
            break;
        }
    }
}