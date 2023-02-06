#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 128

int compareLines(const void *a, const void *b) {
    // Start comparing the lines from the second word
    return strcmp(*(char **)a + strlen(*(char **)a) + 1, *(char **)b + strlen(*(char **)b) + 1);
}

int main(int argc, char *argv[]) {
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    char **lines;
    int i, lineCount;
    // char *filename;

    // Check if the input filename was provided
    /*if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return -1;
    }*/
    //filename = argv[1];

    // Open the input file
    fp = fopen("test.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Read each line of the file and store the lines in an array of strings
    lineCount = 0;
    lines = NULL;
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        if (line[strlen(line) - 1] != '\n') {
            fprintf(stderr, "Line too long\n");
            return 1;
        }
        lineCount++;
        lines = realloc(lines, lineCount * sizeof(char *));
        if (lines == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            return -1;
        }
        lines[lineCount - 1] = malloc(strlen(line) + 1);
        if (lines[lineCount - 1] == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            return -1;
        }
        strcpy(lines[lineCount - 1], line);
    }

    // Sort the lines lexicographically
    qsort(lines, lineCount, sizeof(char *), compareLines);

    // Print the sorted lines
    for (i = 0; i < lineCount; i++) {
        printf("%s", lines[i]);
        free(lines[i]);
    }
    free(lines);

    return 0;
}

if (line[strlen(line) - 1] != '\n') {
fprintf(stderr, "Line too long\n");
printf("String length: %lu", strlen(line));
exit(1);
}
