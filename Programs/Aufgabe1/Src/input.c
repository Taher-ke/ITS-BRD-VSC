/**
 * @file input.c
 * @author Sönke Wulf , Taher Keshavarzian
 * @date Mar 2016
 * 
 * @brief This module implements functions that listen for input.
 */

#include <operations.h>
#include <token.h>
#include <scanner.h>
#include "output.h"

 int process_token(Stack *stack, T_token *token) {
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
			status = prt_3(stack);
			break;
		case CLEAR:
			status = clear(stack);
			break;
		case DOUBLE:
			status = dup(stack);
			//status = test(stack);
			break;
		case SWAP:
			status = swap(stack);
			break;
		case OVERFLOW:
			status= E_OUT_OF_RANGE;
			break;
		default:
			status = E_DEFAULT_ERROR;
	}
	return status;
}

int listen_rpn(Stack *stack) {
	int status = 0;
	// Main Loop
	while(status >= 0) {
		T_token token = next_token();
		status = process_token(stack,&token);
	}
	return status;
}

void listen_restart(int *status) {
	while(1) {
		T_token token = next_token();
		if (token.tok == 'C') {
			*status = 0;
            return;
		}
	}
}