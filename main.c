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
    int line_number = 1;

    while ((line = read_line(file)))
    {
        printf("%-5d %s\n", line_number++, line);
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
    int line_number = 1;

    while (fgets(line, sizeof(line), stdin))
    {
        fprintf(file, "%-5d %s", line_number++, line);
    }

    fclose(file);

    printf("File saved successfully!\n");
}

void find_and_replace(char *filename)
{

    // implementation of find-and-replace function goes here
    // ...
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