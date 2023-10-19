#include <stdio.h>

typedef struct {
    int a;
    char ab[10];
} b;

int main() {
    FILE *f;
    int i;
    b var[2];

    f = fopen("new.c", "wb");
    if (f == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    printf("Enter values in structure\n");
