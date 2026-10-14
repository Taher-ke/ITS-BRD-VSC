/**
 * @file input.c
 * @author Sönke Wulf
 * @date Mar 2016
 * 
 * @brief This module implements functions that listen for input.
 */

 #include <operations.h>
 #include <token.h>
#include <scanner.h>

 int processToken(Stack *stack, T_token *token) {
	int status = -1;
	switch(token->tok) {
		case NUMBER:
			status = number(stack, token->val);
			break;
		case PLUS:
			status = plus(stack);
			break;
		case MINUS:
			status = minus(stack);
			break;
		case MULT:
			status = mult(stack);
			break;
		case DIV:
			status = div(stack);
			break;
		case PRT:
			status = prt(stack);
			break;
		case PRT_ALL:
			status = prtAll(stack);
			break;
		case CLEAR:
			status = clear(stack);
			break;
		case DOUBLE:
			status = d(stack);
			//status = test(stack);
			break;
		case SWAP:
			status = swap(stack);
			break;
		default:
			status = -1;
	}
	return status;
}

int listenRPN(Stack *stack) {
	int status = 0;
	// Main Loop
	while(status >= 0) {
		T_token token = nextToken();
		status = processToken(stack,&token);
	}
	return status;
}

void listenRestart(int *status) {
	while(1) {
		T_token token = nextToken();
		if (token.tok == 'C') {
			*status = 0;
            return;
		}
	}
}