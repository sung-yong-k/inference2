#include <omp.h>
#include <stdio.h>

#define MAX_PEOPLE 10

int main() {
    int num_people = 0;
    int i;

    #pragma omp parallel num_threads(4)
    {
        #pragma omp for
        for (i = 0; i < MAX_PEOPLE; i++) {
            #pragma omp atomic
            num_people++;
        }
    }

    printf("Total number of people: %d\n", num_people);

    return 0;
}