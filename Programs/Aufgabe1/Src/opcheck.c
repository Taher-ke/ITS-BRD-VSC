/**
 * @file opcheck.c
 * @author Sönke Wulf
 * @date Mar 2016
 * 
 * @brief This module checks RPN operations for errors.
 *
 * Error-Codes: 
 * -4 = Division durch Null
 * -5 = Bereichsüberschreitung
 */

 #include <token.h>
 #include <limits.h>

int checkPlus(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return -5;
    }
    return 0;
}

int checkMinus(int a, int b) {
    // b - a
    if ((a < 0 && b > INT_MAX + a) || (a > 0 && b < INT_MIN + a)) {
        return -5; 
    }
    return 0;
}

int checkMult(int a, int b) {
    // Ergebnis null
    if (a == 0 || b == 0) {
        return 0;
    }

    // positives Ergebnis
    if ((a > 0 && b > 0) || (a < 0 && b < 0)) {
        // Betrag berechnen
        unsigned int ua;
        unsigned int ub;
        if (a < 0) {
            ua = (unsigned int) (-a);
        } else {
            ua = (unsigned int) a;
        }
        if (b < 0) {
            ub = (unsigned int) (-b);
        } else {
            ub = (unsigned int) b;
        }
        // max_int prüfen
        if (ua > (unsigned int) INT_MAX / ub) {
            return -5;
        }
    }

    // negatives Ergebnis
    else {
        if (a > 0) { // a positiv, b negativ
            if (b < INT_MIN / a) return -5;
        } 
        else { // a negativ, b positiv
            if (a < INT_MIN / b) return -5;
        }
    }
    return 0;
}

int checkDiv(int a, int b) {
    // div mit 0
    if (a == 0) return -4;
    
    // min_int / -1 Bereichsüberschreitung
    if (b == INT_MIN && a == -1) return -5; 
    
    return 0;
}


// Aufteilung in vier Funktionen
int checkOverflow(int *a, int *b, char type) {
    int status;
    switch(type) {
        case '+':
            status = checkPlus(*a, *b);
            break;
        case '-':
            status = checkMinus(*a, *b);
            break;
        case '*':
            status = checkMult(*a, *b);
            break;
        case '/':
            status = checkDiv(*a, *b);
            break;
        default: 
            status = -1;
    }
    return status;
}

// EOF