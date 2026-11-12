/**
 * @file opcheck.c
 * @author Sönke Wulf , Taher Keshavarzian
 * @date Mar 2016
 *
 * @brief This module checks RPN operations for errors.
 *
 * Error-Codes:
 * E_ZERO_DIVISION = Division durch Null
 * E_OUT_OF_RANGE = Bereichsüberschreitung
 */

#include "output.h"
#include <limits.h>
#include <token.h>

int check_plus(int op1, int op2) {
    // Prüfe auf Bereichsüberschreitung (nach oben und nach unten)
    if ((op2 > 0 && op1 > INT_MAX - op2) || (op2 < 0 && op1 < INT_MIN - op2)) {
        return E_OUT_OF_RANGE;
    }
    return NO_ERROR;
}

int check_minus(int op1, int op2) {
    // Erinnerung Reihenfolge: op2 - op1
    // Prüfe auf Bereichsüberschreitung (nach oben und nach unten) wie bei check_plus()
    if ((op1 < 0 && op2 > INT_MAX + op1) || (op1 > 0 && op2 < INT_MIN + op1)) {
        return E_OUT_OF_RANGE;
    }
    return NO_ERROR;
}

int check_mult(int op1, int op2) {
    // Aufteilung in drei Prüffälle: Ergebnis ist null, positiv oder negativ
    // 1. Ergebnis null
    if (op1 == 0 || op2 == 0) {
        return NO_ERROR;
    }

    // 2. positives Ergebnis
    if ((op1 > 0 && op2 > 0) || (op1 < 0 && op2 < 0)) {
        // Betrag berechnen
        unsigned int u_op1;
        unsigned int u_op2;
        if (op1 < 0) {
            u_op1 = (unsigned int)(-op1);
        } else {
            u_op1 = (unsigned int)op1;
        }
        if (op2 < 0) {
            u_op2 = (unsigned int)(-op2);
        } else {
            u_op2 = (unsigned int)op2;
        }
        // max_int prüfen
        if (u_op1 > (unsigned int)INT_MAX / u_op2) {
            return E_OUT_OF_RANGE;
        }
    }

    // 3. negatives Ergebnis
    else {
        if (op1 > 0) { // a positiv, b negativ
            if (op2 < INT_MIN / op1) {
                return E_OUT_OF_RANGE;
            }
        } else { // a negativ, b positiv
            if (op1 < INT_MIN / op2) {
                return E_OUT_OF_RANGE;
            }
        }
    }
    return NO_ERROR;
}

int check_div(int op1, int op2) {
    // Prüfung auf Division mit 0
    if (op1 == 0) {
        return E_ZERO_DIVISION;
    }

    // MIN_INT / -1 (Prüfung auf Bereichsüberschreitung)
    if (op2 == INT_MIN && op1 == -1) {
        return E_OUT_OF_RANGE;
    }

    return NO_ERROR;
}

// Verteilung auf vier Prüf-Funktionen (je nach Operator)
int check_overflow(int const *op1, int const *op2, char type) {
    int status;
    switch (type) {
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
}

// EOF