    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
    #include <gethostuuid.h>
    #include <time.h>

    #define MAX 129

    struct line {
        char* line;
        char* key;
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

    /*void add(struct list* newlist, char* line, char* key) {
        if (newlist->size == newlist->capacity) {
            resize(newlist);
        }
        newlist->data[newlist->size].line = line;
        newlist->data[newlist->size].key = key;
        newlist->size++;
    }*/

    void add(struct list* newlist, char* line) {
        if (newlist->size == newlist->capacity) {
            resize(newlist);
        }
        newlist->data[newlist->size].line = line;
        newlist->size++;
    }

    void keyword(struct line* newline, int keyword) {
        int count = 1;
        char* c = newline->line;
        char* p = c;
        while (*c != '\0') {
            if (count == keyword) break;
            p = c;
            while (*c != ' ' && *c != '\0') c++;
            while (*c == ' ') c++;

            if(*c != '\0') count++;
        }
        if (keyword > count) c = p;

        char* end = c;
        while (*end != ' ' && *end != '\0') {
            end++;
        }
        int wordsize = end - c;
        char* key = malloc(wordsize + 1);
        if (key == NULL) {
            fprintf(stderr, "Error: Malloc failed\n");
            exit(1);
        }
        for (int i=0; i < wordsize; i++) {
            key[i] = *c;
            c++;
        }
        key[wordsize] = '\0';
        newline->key = key;
        // printf("keyword: %s\n", key);

    }

    int compare(struct line* a, struct line* b) {
        return strcmp(a->key, b->key);
    }

    void cleanup(struct list* newlist) {
        free(newlist->data);
        newlist->size = 0;
        newlist->capacity = 0;
    }

    void test_keyword(struct line* l, char* expected, int i)
    {
        keyword(l, i);
        if (strcmp(l->key, expected)) {
            printf("Error: Key Mismatch Expected %s, Got %s\n", expected, l->key);
        }
    }

    int drive_sort(int argc, char* argv[]) {
        struct list l = {};
        int key = 5;
        init(&l);
        add(&l, "testing string"); //0
        add(&l, "toffee loves you"); //1
        add(&l, "nala is obsessed with trash at the moment"); //2
        add(&l, "stop me from adopting more cats"); //3
        test_keyword(l.data, "string", 3);
        test_keyword(l.data + 1, "you", 4);
        test_keyword(l.data + 1, "loves", 2);
        test_keyword(l.data + 2, "obsessed", 3);
        test_keyword(l.data + 3, "more", 5);
        test_keyword(l.data + 1, "you", 10);
        test_keyword(l.data + 3, "stop", 1);
        test_keyword(l.data + 2, "is", 2);

        /*for (int i=0; i < l.size; i++) {
            keyword(&l.data[i], key);
        }*/
        //qsort(l.data, l.size, sizeof(struct line), (int (*)(const void *, const void *)) compare);

        /*keyword(&l.data[0], 5);
        printf("Should be words\n");

        keyword(&l.data[3], 4);
        printf("Should be adopting\n");

        keyword(&l.data[0], 1);
        printf("Should be testing\n");

        keyword(&l.data[0], 2);
        printf("Should be string\n");

        keyword(&l.data[0], 10); //OOB
        printf("Should be string\n");

        keyword(&l.data[1], 1);
        printf("Should be toffee\n");

        keyword(&l.data[0], 12); //OOB
        printf("Should be string\n");

        keyword(&l.data[2], 3);
        printf("Should be obsessed\n");

        keyword(&l.data[2], 20); //OOB
        printf("Should be moment\n"); */

        /*for (int i = 0; i < l.size; i++) {
            printf("%s\n", l.data[i].line);
        }*/
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

