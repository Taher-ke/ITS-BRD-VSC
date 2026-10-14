/**
 * @file stack.h
 * @author Sönke Wulf
 * @date Mar 2016
 * @brief Header file of stack module.
 */
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
// max stack size
#define STACK_SIZE 11

typedef struct {
	int arr[STACK_SIZE];
	int pos;
} Stack;

void init(Stack *stack);

bool is_empty(Stack *stack);

bool is_full(Stack *stack);

int push(Stack *stack, int value);

int pop(Stack *stack, int *popped);

int peek(Stack *stack, int *peeked);

#endif
// EOF
