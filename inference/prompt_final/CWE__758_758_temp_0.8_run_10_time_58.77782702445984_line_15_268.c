#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int encrypt_file(const char *filename, const char *key);
int decrypt_file(const char *filename, const char *key);

int main() {
    const char* key = "mysecretkey";

    // Create a plaintext file
    FILE *f = fopen("plaintext.txt", "w");
