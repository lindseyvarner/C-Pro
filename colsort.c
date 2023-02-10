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

    void nthword(struct line* newline, int nthword) {
        int count = 1;
        char* c = newline->line;
        char* p = c;
        while (*c != '\0') {
            if (count == nthword) break;
            if (*c == ' ' && count == 1)
            {
                c++;
            }
            p = c;
            while (*c != ' ' && *c != '\0') c++;
            while (*c == ' ')
            {
                c++;
            }
            count++;
        }
        if (nthword > count) c = p;

        char* end = c;
        while (*end != ' ') {
            end++;
        }
        int wordsize = end - c;
        char* key = malloc(wordsize + 1);
        for (int i=0; i < wordsize; i++) {
            key[i] = *c;
            c++;
        }
        key[wordsize] = '\0';
        printf("nthword: %s\n", key);

    }

    void qsort(void* base, size_t nitems, size_t size, int (*compar)(const void *, const void*))
    // void pointer, int numofitems, int size, 

    void cleanup(struct list* newlist) {
        free(newlist->data);
        newlist->size = 0;
        newlist->capacity = 0;
    }

    int drive_sort(int argc, char* argv[]) {
        struct list l = {};
        init(&l);
        add(&l, "testing   string", "something different");
        add(&l, "toffee                loves you", "so does nala");
        add(&l, "nala is   obsessed with trash at the moment", "she has been playing fetch with a tissue ball");
        add(&l, "stop me from     adopting more cats     ", "aaaaaaahhhhhhh");

        nthword(&l.data[3], 4);
        printf("Should be adopting\n");

        nthword(&l.data[0], 1);
        printf("Should be testing\n");

        nthword(&l.data[0], 2);
        printf("Should be string\n");

        nthword(&l.data[0], 10); //OOB
        printf("Should be string\n"); //******

        nthword(&l.data[1], 1);
        printf("Should be toffee\n");

        nthword(&l.data[0], 12); //OOB
        printf("Should be string\n"); //******

        nthword(&l.data[2], 3);
        printf("Should be obsessed\n");

        nthword(&l.data[2], 20); //OOB
        printf("Should be moment\n");

        for (int i = 0; i < l.size; i++) {
            printf("%s\n", l.data[i].line);
        }
        cleanup(&l);

        return 0;
    }


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

