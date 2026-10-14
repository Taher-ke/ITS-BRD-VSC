/**
 * @file output.c
 * @author Sönke Wulf
 * @date Mar 2016
 * 
 * @brief This module offers functions to print to the display
 */

#include <display.h>
#include "errno.h"
#include "string.h"

// Fehlermeldung auf dem Terminal ausgeben
void printError(int *status) {
    clearStdout();
    switch(*status) {
        case -2:
            printStdout("Stack Overflow (2)\n");
            break;
        case -3:
            printStdout("Stack Underflow (3)\n");
            break;
        case -4:
            printStdout("Division durch Null (4)\n");
            break;
        case -5:
            printStdout("Bereichsueberschreitung (5)\n");
            break;
        default:
            //printStdout(strerror(errno));
            printStdout("Unbekannter Fehler\n");
    }
}

// Integer in String umwandeln und auf dem Terminal ausgeben
void printInt(int input)
{
    // Maximale Länge des Integers:
    // (10 Ziffern, 1 minus, 1 \n,  1 Nullterminator) = 13
    char arr[13];
    int isNegative = 0;

    // Negative Zahlen abfangen
    unsigned int pInput;
    if(input < 0) {
        isNegative = 1;
        arr[0] = '-';
        pInput = (unsigned int) -input;
    }
    else {
        pInput = (unsigned int) input;
    }

    // Anzahl Ziffern herausfinden
    int digits = isNegative;
    unsigned int pInput2 = pInput;
    while(pInput2 >= 10)
    {
        pInput2 = pInput2 / 10;
        digits ++;
    }

    // Zeilenumbruch und Nullterminator
    arr[digits + 1] = '\n';
    arr[digits + 2] = '\0';

    // In Ziffern umwandeln
    while(digits >= isNegative)
    {
        arr[digits] = (char) (pInput % 10 + '0');
        pInput = pInput / 10;
        digits --;
    }
    printStdout(arr);
}

void clearOutput(void) {
    clearStdout();
}