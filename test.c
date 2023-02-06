#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 129

int countWords(char* line, int* wordIndices) {
    int count = 0;
    int currentIndex = 0;
    int lineLength = strlen(line);

    for (int i = 0; i < lineLength; i++) {
        if (line[i] != ' ') {
            // If this is the first character of a new word, record its index
            if (currentIndex == 0 || line[currentIndex - 1] == ' ') {
                wordIndices[count] = i;
                count++;
            }
            currentIndex++;
        } else {
            // Delete extra spaces
            while (line[i + 1] == ' ') {
                i++;
            }
        }
    }
    return count;
}

int compare(const void* a, const void* b, int startingWord) {
    char* lineA = *(char**)a;
    char* lineB = *(char**)b;
    int wordIndicesA[MAX];
    int wordIndicesB[MAX];
    int wordCountA = countWords(lineA, wordIndicesA);
    int wordCountB = countWords(lineB, wordIndicesB);
    int startA = wordIndicesA[startingWord];
    int startB = wordIndicesB[startingWord];
    return strcmp(lineA + startA, lineB + startB);
}

int main(int argc, char *argv[]) {
    FILE* fileptr;
    char line[MAX];
    char** lines;
    int i, lineCount;
    char* filename;
    int startingWord = 0;
    int wordIndices[MAX];

    if (argc > 3) {
        fprintf(stderr, "Error: Bad command line parameters\n");
        exit(1);
    }

    if (argc == 2) {
        filename = argv[1];
    } else if (argc == 3) {
        if (argv[1][0] == '-') {
            startingWord = atoi(argv[1] + 1);
            filename = argv[2];
        } else {
            fprintf(stderr, "Error: Bad command line parameters\n");
            exit(1);
        }
    }

    fileptr = fopen(filename, "r");
    if (fileptr == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        exit(1);
    }

    lineCount = 0;
    lines = NULL;
    while (fgets(line, MAX, fileptr) != NULL) {
        /*if (line[strlen(line) - 1] != '\n') {
            fprintf(stderr, "Line too long\n");
            printf("String length: %lu", strlen(line));
            exit(1);
        }*/
        lineCount++;
        lines = realloc(lines, lineCount * sizeof(char*));
        if (lines == NULL) {
            fprintf(stderr, "Error: Malloc failed\n");
            exit(1);
        }
        lines[lineCount - 1] = malloc(strlen(line) + 1);
        if (lines[lineCount - 1] == NULL) {
            fprintf(stderr, "Error: Malloc failed\n");
            exit(1);
        }
        strcpy(lines[lineCount - 1], line);
    }

    qsort(lines, lineCount, sizeof(char*), compare);

    for (i = 0; i < lineCount; i++) {
        int wordCount = countWords(lines[i], wordIndices);
        if (startingWord >= wordCount) {
            startingWord = 0;
        }
        char* wordToCompare = lines[i] + wordIndices[startingWord];
        printf("%s", wordToCompare);
        free(lines[i]);
    }
    free(lines);

    fclose(fileptr);

    return 0;