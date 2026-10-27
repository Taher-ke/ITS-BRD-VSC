/**
  ******************************************************************************
  * @file    main.c
  * @author  Sönke Wulf , Taher Keshavarzian
  * @brief   RPN-Taschenrechner
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "init.h"
#include <stdbool.h>

#include "display.h"

#include <stack.h>
#include <input.h>
#include <output.h>

int main(void) {
	// Initialisierung
	initITSboard();		
	int status = 0; // Für Fehler-Codes
	initDisplay();	
	Stack stack;

	/** Loop: 
	1. Stack-Pointer setzen
	2. RPN-Betrieb
	3. Fehlermeldung ausgeben
	4. Auf Neustart warten  */ 
	while(1) {
		init(&stack);
		status = listen_rpn(&stack);
		print_error(&status);
		listen_restart(&status);
	}
	return -1;
}

// EOF
