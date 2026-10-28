/**
 * @file output.c
 * @author Sönke Wulf , Taher Keshavarzian
 * @date Mar 2016
 * 
 * @brief This module offers functions to print to the display
 */

#include <display.h>
#include "output.h"

#define LEN_STR_ARRAY 10
#define DECIMAL_BASE 10

// Fehlermeldung auf dem Terminal ausgeben
void print_error(int const *status) {
    clear_stdout();
    switch(*status) {
        case E_STACK_OVERFLOW:
            print_stdout("Stack Overflow\n");
            break;
        case E_STACK_UNDERFLOW:
            print_stdout("Stack Underflow\n");
            break;
        case E_ZERO_DIVISION:
            print_stdout("Division durch Null\n");
            break;
        case E_OUT_OF_RANGE:
            print_stdout("Bereichsueberschreitung\n");
            break;
        default:
            //printStdout(strerror(errno));
            print_stdout("Unbekannter Fehler\n");
    }
}

// Integer in String umwandeln und auf dem Terminal ausgeben
void print_int(int input)
{
    // Maximale Länge des Integers:
    // (10 Ziffern, 1 minus, 1 \n,  1 Nullterminator) = 13
    char arr[LEN_STR_ARRAY + 3];
    int is_negative = 0;

    // Negative Zahlen abfangen
    unsigned int p_input;
    if(input < 0) {
        is_negative = 1;
        arr[0] = '-';
        p_input = (unsigned int) -input;
    }
    else {
        p_input = (unsigned int) input;
    }

    // Anzahl Ziffern herausfinden
    int digits = is_negative;
    unsigned int p_input2 = p_input;
    while(p_input2 >= LEN_STR_ARRAY)
    {
        p_input2 = p_input2 / LEN_STR_ARRAY;
        digits ++;
    }

    // Zeilenumbruch und Nullterminator
    arr[digits + 1] = '\n';
    arr[digits + 2] = '\0';

    // In Ziffern umwandeln
    while(digits >= is_negative)
    {
        arr[digits] = (char) ((p_input % DECIMAL_BASE) + '0');
        p_input = p_input / DECIMAL_BASE;
        digits --;
    }
    print_stdout(arr);
}

void clear_output(void) {
    clear_stdout();
}