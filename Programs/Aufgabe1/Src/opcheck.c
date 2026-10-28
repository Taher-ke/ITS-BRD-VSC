/**
 * @file opcheck.c
 * @author Sönke Wulf , Taher Keshavarzian
 * @date Mar 2016
 *
 * @brief This module checks RPN operations for errors.
 *
<<<<<<< HEAD
 * Error-Codes:
 * -4 = Division durch Null
 * -5 = Bereichsüberschreitung
 */

#include <limits.h>
#include <token.h>
// ueberprueft, ob Bereichueberschreitung bei Addition auftritt.
int check_plus(int a, int b) {
	if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
		return -5;
	}
	return 0;
}
// ueberprueft, ob Bereichueberschreitung bei Subraktion auftritt.
int check_minus(int a, int b) {
	// b - a
	if ((a < 0 && b > INT_MAX + a) || (a > 0 && b < INT_MIN + a)) {
		return -5;
	}
	return 0;
}

int check_mult(int a, int b) {
	// Ergebnis null
	if (a == 0 || b == 0) {
		return 0;
	}
	// ueberprueft, ob Bereichueberschreitung bei multiplikation auftritt.
	//  positives Ergebnis
	if ((a > 0 && b > 0) || (a < 0 && b < 0)) {
		// Betrag berechnen
		unsigned int ua;
		unsigned int ub;
		if (a < 0) {
			ua = (unsigned int)(-a);
		} else {
			ua = (unsigned int)a;
		}
		if (b < 0) {
			ub = (unsigned int)(-b);
		} else {
			ub = (unsigned int)b;
		}
		// max_int prüfen
		if (ua > (unsigned int)INT_MAX / ub) {
			return -5;
		}
	}

	// negatives Ergebnis
	else {
		if (a > 0) { // a positiv, b negativ
			if (b < INT_MIN / a)
				return -5;
		} else { // a negativ, b positiv
			if (a < INT_MIN / b)
				return -5;
		}
	}
	return 0;
}
// ueberpruefung der Division durch Null
int check_div(int a, int b) {
	// div mit 0
	if (a == 0)
		return -4;
=======
 * Error-Codes: 
 * E_ZERO_DIVISION = Division durch Null
 * E_OUT_OF_RANGE = Bereichsüberschreitung
 */

#include <token.h>
#include <limits.h>
#include "output.h"

int check_plus(int op1, int op2) {
    if ((op2 > 0 && op1 > INT_MAX - op2) || (op2 < 0 && op1 < INT_MIN - op2)) {
        return E_OUT_OF_RANGE;
    }
    return 0;
}

int check_minus(int op1, int op2) {
    // b - a
    if ((op1 < 0 && op2 > INT_MAX + op1) || (op1 > 0 && op2 < INT_MIN + op1)) {
        return E_OUT_OF_RANGE; 
    }
    return 0;
}

int check_mult(int op1, int op2) {
    // Ergebnis null
    if (op1 == 0 || op2 == 0) {
        return 0;
    }

    // positives Ergebnis
    if ((op1 > 0 && op2 > 0) || (op1 < 0 && op2 < 0)) {
        // Betrag berechnen
        unsigned int u_op1;
        unsigned int u_op2;
        if (op1 < 0) {
            u_op1 = (unsigned int) (-op1);
        } else {
            u_op1 = (unsigned int) op1;
        }
        if (op2 < 0) {
            u_op2 = (unsigned int) (-op2);
        } else {
            u_op2 = (unsigned int) op2;
        }
        // max_int prüfen
        if (u_op1 > (unsigned int) INT_MAX / u_op2) {
            return E_OUT_OF_RANGE;
        }
    }

    // negatives Ergebnis
    else {
        if (op1 > 0) { // a positiv, b negativ
            if (op2 < INT_MIN / op1) {return E_OUT_OF_RANGE;}
        } 
        else { // a negativ, b positiv
            if (op1 < INT_MIN / op2) {return E_OUT_OF_RANGE;}
        }
    }
    return 0;
}

int check_div(int op1, int op2) {
    // div mit 0
    if (op1 == 0) {return E_ZERO_DIVISION;}
    
    // min_int / -1 Bereichsüberschreitung
    if (op2 == INT_MIN && op1 == -1) {return E_OUT_OF_RANGE;} 
    
    return 0;
}
>>>>>>> sw-refactoring

	// min_int / -1 Bereichsüberschreitung
	if (b == INT_MIN && a == -1)
		return -5;

	return 0;
}

// Aufteilung in vier Funktionen
<<<<<<< HEAD
int check_overflow(int *a, int *b, char type) {
	int status;
	switch (type) {
	case '+':
		status = check_plus(*a, *b);
		break;
	case '-':
		status = check_minus(*a, *b);
		break;
	case '*':
		status = check_mult(*a, *b);
		break;
	case '/':
		status = check_div(*a, *b);
		break;
	default:
		status = -1;
	}
	return status;
=======
int check_overflow(int const *op1, int const *op2, char type) {
    int status;
    switch(type) {
        case '+':
            status = check_plus(*op1, *op2);
            break;
        case '-':
            status = check_minus(*op1, *op2);
            break;
        case '*':
            status = check_mult(*op1, *op2);
            break;
        case '/':
            status = check_div(*op1, *op2);
            break;
        default: 
            status = E_DEFAULT_ERROR;
    }
    return status;
>>>>>>> sw-refactoring
}

// EOF