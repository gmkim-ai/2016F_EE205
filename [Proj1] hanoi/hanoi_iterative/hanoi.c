#include "hanoi.h"

/* You can add any functions */
int count;

struct Stack
{
    int arrsize;
    int top;
    int *arr;
};

void push(struct Stack *stack, int disk)
{
    if (stack->top == stack->arrsize - 1) return;
    stack->arr[++stack->top] = disk;
}
int pop(struct Stack* stack)
{
    if (stack->top == -1) return 0;
    return stack->arr[stack->top--];
}

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
        hanoi_iterative(n, 'a', 'b', 'c');
        gettimeofday(&end, NULL);
        restime = (double)(end.tv_sec) + (double)(end.tv_usec) / 1000000.0 -
        (double)(start.tv_sec) - (double)(start.tv_usec) / 1000000.0;
        printf("Run time for N=%d: ", n);
        printf("%f\n", restime);
        printf("\n");
    }
}

void hanoi_iterative(int n, char dep, char via, char arr)
{
    int movenum = 1, i;
    count = 1;
    struct Stack* depstack = (struct Stack*)malloc(sizeof(struct Stack));
    depstack->arrsize = n;
    depstack->top = -1;
    depstack->arr = (int*) malloc(depstack->arrsize * sizeof(int));
    struct Stack* viastack = (struct Stack*)malloc(sizeof(struct Stack));
    viastack->arrsize = n;
    viastack->top = -1;
    viastack->arr = (int*) malloc(viastack->arrsize * sizeof(int));
    struct Stack* arrstack = (struct Stack*)malloc(sizeof(struct Stack));
    arrstack->arrsize = n;
    arrstack->top = -1;
    arrstack->arr = (int*) malloc(arrstack->arrsize * sizeof(int));

    if (n % 2 == 0) {
        char temp = via;
        via = arr;
        arr = temp;
    }
    for (i = 0; i < n; i++) {
        movenum = movenum * 2;
    }
    movenum = movenum - 1;

    for (i = n; i >= 1; i--) {
        push(depstack, i);
    }

    for (i = 1; i <= movenum; i++) {
        if (i % 3 == 1) appropriate_move(depstack, arrstack, dep, arr);
        else if (i % 3 == 2) appropriate_move(depstack, viastack, dep, via);
        else if (i % 3 == 0) appropriate_move(viastack, arrstack, via, arr);
    }
    free(depstack->arr);
    free(depstack);
    free(viastack->arr);
    free(viastack);
    free(arrstack->arr);
    free(arrstack);
}

void appropriate_move(struct Stack *stack1, struct Stack *stack2, char one, char two)
{
    int topdisk1 = pop(stack1);
    int topdisk2 = pop(stack2);

    if (topdisk1 == 0) {
        push(stack1, topdisk2);
        printf("%d: Disk %d from %c to %c\n", count, topdisk2, two, one);
        count++;
    }
    else if (topdisk2 == 0) {
        push(stack2, topdisk1);
        printf("%d: Disk %d from %c to %c\n", count, topdisk1, one, two);
        count++;
    }
    else if (topdisk1 > topdisk2) {
        push(stack1, topdisk1);
        push(stack1, topdisk2);
        printf("%d: Disk %d from %c to %c\n", count, topdisk2, two, one);
        count++;
    }
    else {
        push(stack2, topdisk2);
        push(stack2, topdisk1);
        printf("%d: Disk %d from %c to %c\n", count, topdisk1, one, two);
        count++;
    }
}
