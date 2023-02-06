#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 128

int compare(const void* a, const void* b) {
    return strcmp(*(char**)a, *(char**)b);
}

int main(int argc, char *argv[]) {
    FILE* fileptr;
    char line[MAX];
    char** lines;
    int i, lineCount;
    char* filename;

    // Check if the input filename was provided
    /*if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return -1;
    }*/

    // Get the input filename from the command line argument
    filename = argv[1];

    // Open the input file
    fileptr = fopen(filename, "r");
    if (fileptr == NULL) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        return 1;
    }

    // Read each line of the file and store the lines in an array of strings
    lineCount = 0;
    lines = NULL;
    while (fgets(line, MAX, fileptr) != NULL) {
        lineCount++;
        lines = realloc(lines, lineCount * sizeof(char*));
        if (lines == NULL) {
            fprintf(stderr, "Malloc failed\n");
            return 1;
        }
        lines[lineCount - 1] = malloc(strlen(line) + 1);
        if (lines[lineCount - 1] == NULL) {
            fprintf(stderr, "Malloc failed\n");
            return 1;
        }
        strcpy(lines[lineCount - 1], line);
    }

    // Sort the lines lexicographically
    qsort(lines, lineCount, sizeof(char*), compare);

    // Print the sorted lines
    for (i = 0; i < lineCount; i++) {
        printf("%s", lines[i]);
        free(lines[i]);
    }
    free(lines);

    // Close the file
    fclose(fileptr);

    return 0;
}
