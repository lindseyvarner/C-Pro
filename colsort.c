    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
    #include <gethostuuid.h>
    #include <time.h>

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
        if (newlist->data == NULL) {
            fprintf(stderr, "Error: malloc failed\n");
            exit(1);
        }
    }

    void resize(struct list* newlist) {
        int capacity = newlist->capacity*2;
        newlist->capacity = capacity;
        newlist->data = realloc(newlist->data, sizeof(struct line)*capacity);
        if (newlist->data == NULL) {
            fprintf(stderr, "Error: malloc failed\n");
            exit(1);
        }
    }

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
        while (*c == ' ') c++;
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

    }

    int compare(struct line* a, struct line* b)
    {
        return strcmp(a->key, b->key);
    }

    void cleanup(struct list* newlist)
    {
        free(newlist->data);
        newlist->size = 0;
        newlist->capacity = 0;
    }

    void test_keyword(struct line* l, char* expected, int i)
    {
        keyword(l, i);
        if (strcmp(l->key, expected)) {
            printf("Error: Keyword expected: %s; Keyword returned: %s\n", expected, l->key);
        }
    }

    struct list open_file(int column, char* filename)
    {
        FILE* fileptr = fopen(filename, "r");
        fopen(filename, "r");
        if (NULL == fileptr) {
            printf("Error: Cannot open file %s\n", filename);
            exit(1);
        }
        struct list l = {};
        init(&l);

        char* temp = malloc((sizeof(char) * 129));
        if (temp == NULL) {
            fprintf(stderr, "Error: Malloc failed\n");
            exit(1);
        }

        while (fgets(temp, 129, fileptr) != NULL) {
            int count = 0;
            char* data = malloc((sizeof(char) * 129));
            if (data == NULL) {
                fprintf(stderr, "Error: Malloc failed\n");
                exit(1);
            }
            for (int i = 0; i < 128; i++) {
                if (temp[i] == '\n' || temp[i] == '\r') count++;
            }
            if (count == 0) {
                fprintf(stderr, "Line too long\n");
                exit(1);
            }
            for (int i = 0; i < strlen(temp); i++) {
                data[i] = temp[i];
            }
            add(&l, data);

            keyword(&l.data[l.size-1], column);
        }

        fclose(fileptr);

        return l;
    }

    int drive_sort(int argc, char* argv[])
    {
        char *filename = NULL;
        int column = 1;
        if (argc > 3) {
            fprintf(stderr, "Error: Bad command line parameters\n");
            exit(1);
        }
        if (argc == 3) {
            char* c = argv[1];
            while (*c == '-') c++;
            column = atoi(c);
            filename = argv[2];
        }
        else if (argc == 2) filename = argv[1];

        struct list l = open_file(column, filename);

        qsort(l.data, l.size, sizeof(struct line), (int (*)(const void *, const void *)) compare);

        for (int i = 0; i < l.size; i++) {
            printf("%s\n", l.data[i].line);
        }
        cleanup(&l);

        return 0;
    }
    
    int main(int argc, char *argv[])
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

