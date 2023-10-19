#include <stdio.h>
#include <stdlib.h>

void sort(int *arr, int cnt, int **low, int **high);

int main(void)
{
    int array[] = {1, -1, 2, 3, 4, -5, 6};
    int cnt = sizeof(array)/sizeof(array[0]);
    int *neg, *pos;
    int **low = &neg;
    int **high = &pos;

    neg = (int*) malloc(sizeof(int) * 2);
    pos = (int*) malloc(sizeof(int) * 5);

    sort(array, cnt, low, high);

    printf("Positive Numbers:\n");
    for(int i = 0; i < 5; i++)
    {
        printf("%3d\n", pos[i]);
    }

    printf("\nNegative Numbers:\n");
    for(int i = 0; i < 2; i++)
    {
        printf("%3d\n", neg[i]);
    }

    free(neg);
    free(pos);

    return 0;
}

void sort(int *arr, int cnt, int **low, int **high)
{
    int negIndex = 0;
    int posIndex = 0;

    for (int i = 0; i < cnt; i++)
    {
        if (arr[i] < 0)
        {
            (*low)[negIndex] = arr[i];
            negIndex++;
        }
        else
        {
            (*high)[posIndex] = arr[i];
            posIndex++;
        }
    }
}