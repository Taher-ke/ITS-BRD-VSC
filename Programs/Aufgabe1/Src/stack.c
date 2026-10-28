/**
 * @file stack.c
 * @author Sönke Wulf , Taher Keshavarzian
 * @date Mar 2016
 *
 * @brief This module implements a stack.
 */

#include "stack.h"
#include <stdbool.h>

// set initial posistion
void init(Stack *stack) { stack->pos = 0; }

// check if empty
bool is_empty(Stack *stack) { return (stack->pos == 0); }

// check if full
bool is_full(Stack *stack) { return (stack->pos >= STACK_SIZE - 1); }

// push on stack
int push(Stack *stack, int value) {
	if (is_full(stack)) {
		return -2;
	}
	stack->arr[stack->pos++] = value;
	return 0;
}

// pop from stack
int pop(Stack *stack, int *popped) {
	if (is_empty(stack)) {
		return -3;
	}

	*popped = stack->arr[--stack->pos];
	return 0;
}

// show top element
int peek(Stack *stack, int *peeked) {
	if (is_empty(stack)) {
		return -3;
	}
	*peeked = stack->arr[stack->pos - 1];
	return 0;
}

// EOF