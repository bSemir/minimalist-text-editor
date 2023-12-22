#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void edit(char buffer[BUFFER_SIZE], int line);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file for reading");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    size_t bytesRead = fread(buffer, 1, BUFFER_SIZE - 1, fp);
    buffer[bytesRead] = '\0';  // null-terminate the buffer
    printf("Content is shown below:\n%s\n", buffer);

    fclose(fp);

    int curr_line = 0;
    printf("Enter the line number to edit: ");
    if (scanf("%d", &curr_line) != 1) {
        fprintf(stderr, "Invalid input for line number\n");
        return EXIT_FAILURE;
    }

    edit(buffer, curr_line);

    FILE *f = fopen(argv[1], "w");
    if (f == NULL) {
        perror("Error opening file for writing");
        return EXIT_FAILURE;
    }

    if (fwrite(buffer, 1, strlen(buffer), f) != strlen(buffer)) {
        perror("Error writing to file");
        fclose(f);
        return EXIT_FAILURE;
    }

    fclose(f);
    printf("File successfully edited and saved.\n");

    return EXIT_SUCCESS;
}

void edit(char buffer[BUFFER_SIZE], int line) {
    if (line < 0) {
        fprintf(stderr, "Invalid line number\n");
        exit(EXIT_FAILURE);
    }

    char *line_start = buffer;
    for (int i = 0; i < line; i++) {
        line_start = strchr(line_start, '\n');
        if (line_start == NULL) {
            fprintf(stderr, "Line number exceeds the total number of lines in the file.\n");
            exit(EXIT_FAILURE);
        }
        line_start++;  // move to the next character after '\n'
    }

    char *line_end = strchr(line_start, '\n');
    if (line_end == NULL) {
        fprintf(stderr, "Error locating the end of the line.\n");
        exit(EXIT_FAILURE);
    }

    char line_after[BUFFER_SIZE];
    strcpy(line_after, line_end + 1);  // save the content after the edited line

    printf("Enter the new line: ");
    if (scanf(" %[^\n]", line_start) != 1) {
        fprintf(stderr, "Error reading the new line.\n");
        exit(EXIT_FAILURE);
    }

    strcat(line_start, "\n");  // append newline character to the modified line
    strcat(buffer, line_after);  // append the content after the edited line
}