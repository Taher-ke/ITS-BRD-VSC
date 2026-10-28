/**
 * @file operations.c
 * @author Sönke Wulf , Taher Keshavarzian
 * @date Mar 2016
 *
 * @brief This module implements RPN operations.
 */

#include "limits.h"
#include "stack.h"
#include <opcheck.h>
#include <output.h>
#include <token.h>

// poppt zwei oberste Elemente, ueberpruft Overflow und pusht die Summe auf den
// Stack.
int plus(Stack *stack) {
	int status;
	int popped1;
	int popped2;
	if ((status = pop(stack, &popped1)) < 0 ||
		 (status = pop(stack, &popped2)) < 0 ||
		 (status = check_overflow(&popped1, &popped2, PLUS)) < 0 ||
		 (status = push(stack, popped1 + popped2)) < 0) {
		return status;
	}
	return 0;
}
// poppt zwei oberste Elemente, ueberpruft Overflow und pusht die Subraktion auf
// den Stack.
int minus(Stack *stack) {
	int status;
	int popped1;
	int popped2;
	if ((status = pop(stack, &popped1)) < 0 ||
		 (status = pop(stack, &popped2)) < 0 ||
		 (status = check_overflow(&popped1, &popped2, MINUS)) < 0 ||
		 (status = push(stack, popped2 - popped1)) < 0) {
		return status;
	}
	return 0;
}
// poppt zwei oberste Elemente, ueberpruft Overflow und pusht die Multiplikation
// auf den Stack.
int mult(Stack *stack) {
	int status;
	int popped1;
	int popped2;
	if ((status = pop(stack, &popped1)) < 0 ||
		 (status = pop(stack, &popped2)) < 0 ||
		 (status = check_overflow(&popped1, &popped2, MULT)) < 0 ||
		 (status = push(stack, popped1 * popped2)) < 0) {
		return status;
	}
	return 0;
}
// poppt zwei oberste Elemente, ueberpruft Overflow und pusht die Division auf
// den Stack.
int div(Stack *stack) {
	int status;
	int popped1;
	int popped2;
	if ((status = pop(stack, &popped1)) < 0 ||
		 (status = pop(stack, &popped2)) < 0 ||
		 (status = check_overflow(&popped1, &popped2, DIV)) < 0 ||
		 (status = push(stack, popped2 / popped1)) < 0) {
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
	print_int(peeked);
	return 0;
}

// die drei obersten Werte auf dem Stack ausgeben
// Dazu wird ein zweiter Stack erzeugt und die Werte werden beim Umstapeln
// ausgegeben
int prt_3(Stack *stack) {
	int status;
	Stack stack2;
	init(&stack2);
	int counter = 0;
	int val;
	while (!is_empty(stack) && counter < 3) {
		if ((status = pop(stack, &val)) < 0 ||
			 (status = push(&stack2, val)) < 0) {
			return status;
		}
		print_int(val);
		counter = counter + 1;
	}
	while (!is_empty(&stack2)) {
		if ((status = pop(&stack2, &val)) < 0 ||
			 (status = push(stack, val)) < 0) {
			return status;
		}
	}
	return 0;
}

// Stack und Ausgabe auf dem Terminal leeren
int clear(Stack *stack) {
	init(stack);
	clear_output();
	return 0;
}

// oberstes Element auf dem Stack duplizieren
int dup(Stack *stack) {
	int status;
	int peeked;
	if ((status = peek(stack, &peeked)) < 0 ||
		 (status = push(stack, peeked)) < 0) {
		return status;
	}
	return 0;
}

// Die beiden obersten Elemente auf dem Stack tauschen
int swap(Stack *stack) {
	int status;
	int val1;
	int val2;
	if ((status = pop(stack, &val1)) < 0 || (status = pop(stack, &val2)) < 0 ||
		 (status = push(stack, val1)) < 0 || (status = push(stack, val2)) < 0) {
		return status;
	}
	return 0;
}

// Zahl auf dem Stack ablegen
int number(Stack *stack, int number) {
	int status;
	if ((status = push(stack, number)) < 0) {
		return status;
	}
	return 0;
}

int test(Stack *stack) {
	int status;
	if ((status = push(stack, INT_MIN)) < 0 || (status = push(stack, -1)) < 0 ||
		 (status = div(stack)) < 0) {
		return status;
	}
	return 0;
}

/* // alle Werte auf dem Stack ausgeben
// Dazu wird ein zweiter Stack erzeugt und alle Werte beim Umstapeln ausgegeben
int prtAll(Stack *stack) {
	 int status;
	 Stack stack2;
	 init(&stack2);
	 int val;
	 while(!is_empty(stack)) {
		  if(
				(status = pop(stack, &val)) < 0 ||
				(status = push(&stack2, val)) < 0
				) {
				return status;
		  }
		  print_int(val);
	 }
	 while(!is_empty(&stack2)) {
		  if(
				(status = pop(&stack2, &val)) < 0 ||
				(status = push(stack, val)) < 0
				) {
				return status;
		  }
	 }
	 return 0;
} */

// EOF