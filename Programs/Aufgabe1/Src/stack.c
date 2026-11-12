/**
 * @file stack.c
 * @author Sönke Wulf , Taher Keshavarzian
 * @date Mar 2016
 *
 * @brief This module implements a stack.
 */

#define EMPTY_STACK 0

#include "stack.h"
#include "output.h"
#include <stdbool.h>

// set initial posistion
void init(Stack *stack) { stack->pos = EMPTY_STACK; }

// check if empty
bool is_empty(Stack *stack) { return (stack->pos == EMPTY_STACK); }

// check if full
bool is_full(Stack *stack) { return (stack->pos >= STACK_SIZE - 1); }

// push on stack
int push(Stack *stack, int value) {
    if (is_full(stack)) {
        return E_STACK_OVERFLOW;
    }
    stack->arr[stack->pos++] = value;
    return NO_ERROR;
}

// pop from stack
int pop(Stack *stack, int *popped) {
    if (is_empty(stack)) {
        return E_STACK_UNDERFLOW;
    }

    *popped = stack->arr[--stack->pos];
    return NO_ERROR;
}

// show top element
int peek(Stack *stack, int *peeked) {
    if (is_empty(stack)) {
        return E_STACK_UNDERFLOW;
    }
    *peeked = stack->arr[stack->pos - 1];
    return NO_ERROR;
}

// EOF