#include "hanoi.h"

/* You can add any functions */
int count;

void hanoi(char *input)
{
    struct timeval start, end;
    double restime;
    FILE *fpin;
    char str[4][10];
    int n;

    fpin = fopen(input, "r");
    fscanf(fpin, "%s %s %s %s", str[0], str[1], str[2], str[3]);
    if (strcmp(str[0], "Heights") || strcmp(str[1], "of") || strcmp(str[2], "Hanoi") || strcmp(str[3], "towers:")) {
        fprintf(stderr, "Input file should be start with 'Heights of Hanoi towers:'\n");
        return;
    }

    while(fscanf(fpin, "%d", &n) != EOF) {
        count = 1;
        printf("N=%d,\n", n);
        gettimeofday(&start, NULL);
        hanoi_recursive(n, 'a', 'b', 'c');
        gettimeofday(&end, NULL);
        restime = (double)(end.tv_sec) + (double)(end.tv_usec) / 1000000.0 -
        (double)(start.tv_sec) - (double)(start.tv_usec) / 1000000.0;
        printf("Run time for N=%d: ", n);
        printf("%f\n", restime);
        printf("\n");
    }
}

void hanoi_recursive(int n, char dep, char via, char arr)
{
    if (n == 1) {
        printf("%d: Disk 1 from %c to %c\n", count, dep, arr);
        count++;
        return;
    }
    hanoi_recursive(n-1, dep, arr, via);
    printf("%d: Disk %d from %c to %c\n", count, n, dep, arr);
    count++;
    hanoi_recursive(n-1, via, dep, arr);
}
