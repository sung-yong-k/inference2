#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *ptr;
    ptr = fopen("D:\\f4.txt", "r+");

    if (ptr == NULL) {
        printf("File can't be opened");
        exit(0);
    }

    char ch = fgetc(ptr);