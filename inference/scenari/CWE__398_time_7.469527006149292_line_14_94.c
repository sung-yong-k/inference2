#include <stdio.h>

void squeeze(char s1[], char s2[]);

int main()
{
    char s1[] = "string";
    char s2[] = "spring";
    squeeze(s1, s2);
    printf("%s", s1);
    return 0;
}

