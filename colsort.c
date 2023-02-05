#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    /*for (int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }*/
    FILE* ptr;
    char ch;
    ptr = fopen("test.txt", "r");
    if (NULL == ptr) {
        printf("file can't be opened \n");
       }

    printf("content of this file are \n");
    do {
        ch = fgetc(ptr);
        printf("%c", ch);
        } while (ch != EOF);
    fclose(ptr);
    // printf("Hello, World!");
    return 0;
}