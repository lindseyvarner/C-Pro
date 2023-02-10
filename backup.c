#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gethostuuid.h>
#include <time.h>

#define MAX 129

struct line {
    char *line;
    char *key;
};

struct list {
    int size;
    int capacity;
    struct line* data;
};

void init(struct list* newlist) {
    newlist->size = 0;
    newlist->capacity = 1000;
    newlist->data = malloc(sizeof(struct line)*1000);
}

void resize(struct list* newlist) {
    int capacity = newlist->capacity*2;
    newlist->capacity = capacity;
    newlist->data = realloc(newlist->data, sizeof(struct line)*capacity);
}

void add(struct list* newlist, char* line, char* key) {
    if (newlist->size == newlist->capacity) {
        resize(newlist);
    }
    newlist->data[newlist->size].line = line;
    newlist->data[newlist->size].key = key;
    newlist->size++;
}

/* void keyword(struct line* newline, int keyword) {
    newline->key = keyword;//feed in nth word from newline
    newline->line;
} */

void cleanup(struct list* newlist) {
    free(newlist->data);
    newlist->size = 0;
    newlist->capacity = 0;
}

int compare(const void* a, const void* b) {
    struct line* l1 = (struct line*)a;
    struct line* l2 = (struct line*)b;
    return strcmp(l1->key, l2->key);
}

int drive_sort(int argc, char* argv[]) {
    FILE* fileptr;
    char line[MAX];
    struct line* lines;
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

    /*lineCount = 0;
    lines = NULL;
    while (fgets(line, MAX, fileptr) != NULL) {
        if (line[strlen(line) - 1] != '\n') {
            fprintf(stderr, "Line too long\n");
            printf("String length: %lu", strlen(line));
            exit(1);
        }*/

    struct list l = {};
    while (fgets(line, MAX, fileptr) != NULL) {
        if (line[strlen(line) - 1] != '\n') {
            fprintf(stderr, "Line too long\n");
            exit(1);
        } else {
            line[strlen(line) - 1] = '\n'; // remove the newline character
            char* key = strdup(line);

            for (i = startingWord; i < strlen(key); i++) {
                if (isspace(*(key + i))) {
                    *(key + i) = '\0';
                    break;
                }
            }
            add(&l, strdup(line), key);
        }
    }


    fclose(fileptr);

    // sort the list by the key
    qsort(l.data, l.size, sizeof(struct line), compare);

    // print out the sorted list
    for (i = 0; i < l.size; i++) {
        printf("%s\n", l.data[i].line);
    }

    cleanup(&l);

    return 0;
}

/* init(&l);
        add(&l, "testing string", "something different");
        add(&l, "toffee loves you", "so does nala");
        add(&l, "nala is obsessed with trash at the moment", "she has been playing fetch with a tissue ball");
        add(&l, "stop me from adopting more cats", "aaaaaaahhhhhhh");

        for (i = 0; i < l.size; i++) {
            printf("%s\n", l.data[i].line);
        }

        return 0;
    } */



// Finds nth space in array
/*

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

int compare(const void* a, const void* b) {
    struct line* l1 = (struct line*)a;
    struct line* l2 = (struct line*)b;
    return strcmp(l1->key, l2->key);
}

int drive_sort(int argc, char* argv[]) {
    FILE* fileptr;
    char line[MAX];
    struct line* lines;
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
        if (line[strlen(line) - 1] != '\n') {
            fprintf(stderr, "Line too long\n");
            printf("String length: %lu", strlen(line));
            exit(1);
        }
        lineCount++;
        lines = realloc(lines, lineCount * sizeof(struct line));
        if (lines == NULL) {
            fprintf(stderr, "Error: Malloc failed\n");
            exit(1);
        }
        lines[lineCount - 1].line = malloc(strlen(line) + 1);
        if (lines[lineCount - 1].line == NULL) {
            fprintf(stderr, "Error: Malloc failed\n");
            exit(1);
        }
        strcpy(lines[lineCount - 1].line, line);

        int wordCount = countWords(line, wordIndices);
        if (wordCount <= startingWord) {
            startingWord = 0;
        }
        char* wordToCompare = line + wordIndices[startingWord];
        lines[lineCount - 1].key = malloc(strlen(wordToCompare) + 1);
        if (lines[lineCount - 1].key == NULL) {
            fprintf(stderr, "Error: Malloc failed\n");
            exit(1);
        }
        strcpy(lines[lineCount - 1].key, wordToCompare);
    }
        qsort(lines, lineCount, sizeof(struct line), compare);

        for (i = 0; i < lineCount; i++) {
            printf("%s\n", lines[i].line);
            free(lines[i].line);
            free(lines[i].key);
        }
        free(lines);

        fclose(fileptr);

        return 0;
    }*/

int main(int argc, char *argv[]) {
    int ret = 0;
    double time;
    struct timespec s;
    struct timespec e;
    clock_gettime(CLOCK_MONOTONIC, &s);
    ret = drive_sort(argc, argv);
    clock_gettime(CLOCK_MONOTONIC, &e);
    time = e.tv_sec - s.tv_sec + (e.tv_nsec - s.tv_nsec) / 1e9;
    fprintf(stderr, "time: %lfs\n", time);
    return ret;
}

