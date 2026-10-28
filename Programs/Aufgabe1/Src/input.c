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
/**
* @brief Wertet ein Token aus und führt den zugehörigen RPN-Befehl aus.
*
* Zahlen werden auf den Stack gelegt. Andere Token lösen die
* entsprechende Rechen-, Stack- oder Ausgabefunktion aus.
*
* @param stack Zeiger auf den Operandenstack.
* @param token Zeiger auf das zu verarbeitende Token.
* @return 0 bei Erfolg oder ein negativer Fehlercode.
*         -5 bei einer Bereichsüberschreitung,
*         -1 bei einem unbekannten Token.
*/

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
			status= -5;
			break;
		default:
			status = -1;
	}
	return status;
}
/**
 * @brief Liest und verarbeitet RPN-Token, bis ein Fehler auftritt.
 *
 * @param stack Zeiger auf den verwendeten Operandenstack.
 * @return Negativer Fehlercode, der die Verarbeitung beendet hat.
 */
int listen_rpn(Stack *stack) {
	int status = 0;
	// Token lesen und verarbeiten, solange kein Fehler vorliegt.
	while(status >= 0) {
		T_token token = nextToken();
		status = process_token(stack,&token);
	}
	return status;
}
/**
 * @brief Wartet auf die Taste C und setzt den Fehlerstatus auf 0.
 *
 * Andere Token werden ignoriert. Nach C kehrt die Funktion zurück,
 * damit das Hauptprogramm den Rechner neu initialisieren kann.
 *
 * @param status Zeiger auf den zurückzusetzenden Fehlerstatus.
 */
void listen_restart(int *status) {
	while(1) {
		T_token token = nextToken();
		if (token.tok == 'C') {
			*status = 0;
            return;
		}
	}
}