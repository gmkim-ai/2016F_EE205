#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
#include<string.h>

void hanoi(char *);
void hanoi_iterative(int, char, char, char);
struct Stack;
void push(struct Stack *, int);
int pop(struct Stack *);
void appropriate_move(struct Stack *, struct Stack *, char, char);
