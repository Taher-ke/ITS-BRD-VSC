/**
 * @file operations.c
 * @author Sönke Wulf 
 * @date Mar 2016
 * 
 * @brief This module implements RPN operations.
 */
 
#include "stack.h"
#include <output.h>
#include <token.h>
#include <opcheck.h>
#include "limits.h"


int plus(Stack *stack) {
    int status;
    int popped1;
    int popped2;
    if (
        (status = pop(stack, &popped1)) < 0 || 
        (status = pop(stack, &popped2)) < 0 ||
        (status = checkOverflow(&popped1, &popped2, PLUS)) < 0 ||
        (status = push(stack, popped1 + popped2)) < 0
    ) {
        return status;
    }
    return 0;
}

int minus(Stack *stack) {
    int status;
    int popped1;
    int popped2;
    if (
        (status = pop(stack, &popped1)) < 0 || 
        (status = pop(stack, &popped2)) < 0 ||
        (status = checkOverflow(&popped1, &popped2, MINUS)) < 0 ||
        (status = push(stack, popped2 - popped1)) < 0
    ) {
        return status;
    }
    return 0;
}

int mult(Stack *stack) {
    int status;
    int popped1;
    int popped2;
    if (
        (status = pop(stack, &popped1)) < 0 || 
        (status = pop(stack, &popped2)) < 0 ||
        (status = checkOverflow(&popped1, &popped2, MULT)) < 0 ||
        (status = push(stack, popped1 * popped2)) < 0
    ) {
        return status;
    }
    return 0;
}

int div(Stack *stack) {
    int status;
    int popped1;
    int popped2;
    if (
        (status = pop(stack, &popped1)) < 0 || 
        (status = pop(stack, &popped2)) < 0 ||
        (status = checkOverflow(&popped1, &popped2, DIV)) < 0 ||
        (status = push(stack, popped2 / popped1)) < 0
    ) {
        return status;
    }
    return 0;
}

// obersten Wert auf dem Stack ausgeben
int prt(Stack *stack) {
    int status;
    int peeked;
    if ((status = peek(stack, &peeked)) < 0) {
        return status;
    }
    printInt(peeked);
    return 0;
}
// alle Werte auf dem Stack ausgeben
// Dazu wird ein zweiter Stack erzeugt und alle Werte beim Umstapeln ausgegeben
int prtAll(Stack *stack) {
    int status;
    Stack stack2;
    init(&stack2);
    int val;
    while(!isEmpty(stack)) {
        if(
            (status = pop(stack, &val)) < 0 ||
            (status = push(&stack2, val)) < 0
            ) {
            return status;
        }
        printInt(val);
    }
    while(!isEmpty(&stack2)) {
        if(
            (status = pop(&stack2, &val)) < 0 ||
            (status = push(stack, val)) < 0
            ) {
            return status;
        }
    }
    return 0;
}

// Stack und Ausgabe auf dem Terminal leeren
int clear(Stack *stack)
{
    init(stack);
    clearOutput();
    return 0;
}

// oberstes Element auf dem Stack duplizieren
int d(Stack *stack) {
    int status;
    int peeked;
    if (
        (status = peek(stack, &peeked)) < 0 || 
        (status = push(stack, peeked)) < 0
    ) {
        return status;
    }
    return 0;
}

// Die beiden obersten Elemente auf dem Stack tauschen
int swap(Stack *stack)
{
    int status;
    int val1;
    int val2;
    if (
        (status = pop(stack, &val1)) < 0 ||
        (status = pop(stack, &val2)) < 0 ||
        (status = push(stack, val1)) < 0 ||
        (status = push(stack, val2)) < 0
    ) {
        return status;
    }
    return 0;
}

// Zahl auf dem Stack ablegen
int number(Stack *stack, int number)
{
    int status;
    if ((status = push(stack, number)) < 0) {
        return status;
    }
    return 0;
}

int test(Stack *stack)
{
    int status;
    if (
        (status = push(stack, INT_MIN)) < 0 ||
        (status = push(stack, -1)) < 0 ||
        (status = div(stack)) < 0
    ) {
        return status;
    }
    return 0;
}

// EOF