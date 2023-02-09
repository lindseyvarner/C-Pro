#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gethostuuid.h>
#include <time.h>

#define MAX_LENGTH 129
#define MAX_LINES 100
#define MAX_WORD_LENGTH 20


int compare_strings(const void *a, const void *b) {
    const char *word1 = *(const char **)a + strlen(*(const char **)a);
    const char *word2 = *(const char **)b + strlen(*(const char **)b);
    while (*word1 != ' ' && word1 > *(const char **)a) {
        word1--;
    }
    while (*word2 != ' ' && word2 > *(const char **)b) {
        word2--;
    }
    return strcmp(word1, word2);
}

void sort_lines(char lines[][MAX_LENGTH], int n) {
    qsort(lines, n, sizeof(lines[0]), compare_strings);
}

int drive_sort(const char *filename, const char *word) {
    FILE *fp;
    char line[MAX_LENGTH];
    char lines[MAX_LINES][MAX_LENGTH];
    int n = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    while (fgets(line, MAX_LENGTH, fp) != NULL && n < MAX_LINES) {
        line[strcspn(line, "\n")] = 0;
        strcpy(lines[n], line);
        n++;
    }

    fclose(fp);

    sort_lines(lines, n);

    for (int i = 0; i < n; i++) {
        const char *line = lines[i];
        const char *word_start = line + strlen(line);
        while (*word_start != ' ' && word_start > line) {
            word_start--;
        }
        if (strncmp(word_start, word, strlen(word)) == 0) {
            printf("Line: %s\n", line);
        }
    }
    return 0;
}




int main(int argc, char* argv[])
{
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
