/**
 * @file output.c
 * @author Sönke Wulf , Taher Keshavarzian
 * @date Mar 2016
 *
 * @brief This module offers functions to print to the display
 */

/*
 *@return
 *
 */

#include "errno.h"
#include "string.h"
#include <display.h>

// Fehlermeldung auf dem Terminal ausgeben
void print_error(int *status) {
	clearStdout();
	switch (*status) {
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
		// printStdout(strerror(errno));
		printStdout("Unbekannter Fehler\n");
	}
}

// Integer in String umwandeln und auf dem Terminal ausgeben
void print_int(int input) {
	// Maximale Länge des Integers:
	// (10 Ziffern, 1 minus, 1 \n,  1 Nullterminator) = 13
	char arr[13];
	int is_negative = 0;

	// Negative Zahlen abfangen
	unsigned int p_input;
	if (input < 0) {
		is_negative = 1;
		arr[0] = '-';
		p_input = (unsigned int)-input;
	} else {
		p_input = (unsigned int)input;
	}

	// Anzahl Ziffern herausfinden
	int digits = is_negative;
	unsigned int p_input2 = p_input;
	while (p_input2 >= 10) {
		p_input2 = p_input2 / 10;
		digits++;
	}

	// Zeilenumbruch und Nullterminator
	arr[digits + 1] = '\n';
	arr[digits + 2] = '\0';

	// In Ziffern umwandeln
	while (digits >= is_negative) {
		arr[digits] = (char)(p_input % 10 + '0');
		p_input = p_input / 10;
		digits--;
	}
	printStdout(arr);
}

void clear_output(void) { clearStdout(); }