/**
 ******************************************************************************
 * @file    main.c
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   GS Praktikumsaufgabe 3: BMP-Bilder ueber USB empfangen,
 *          dekodieren, ggf. verkleinern und auf dem LCD darstellen.
 *
 *          Ablauf:
 *            1. Board, Takt (168 MHz fuer USB), Timer und LCD initialisieren
 *            2. Transport waehlen (S7 = USB-CDC, S6 = UART-via-USB) und
 *               Handshake mit dem Python-Programm (init_input)
 *            3. Endlosschleife: Bild darstellen (handle_picture), danach auf
 *               S5 warten -- S5 fordert das naechste Bild an
 *          Fehler meldet der Error-Handler auf dem LCD; die Meldung bleibt
 *          bis zum Druck auf S5 stehen.
 ******************************************************************************
 */

#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "bmpApp.h"
#include "error.h"
#include "fontsFLASH.h"
#include "init.h"
#include "input.h"
#include "usb_clock_config.h"
#include <timer.h>

int main(void) {
   initITSboard();               // Initialisierung des ITS Boards
   switch_to_168_MHz();          // USB braucht ein Vielfaches von 42 MHz
   initTimer();
   GUI_init(DEFAULT_BRIGHTNESS); // Initialisierung des LCD
   TP_Init(false);               // Initialisierung des Touch-Controllers
   if (!checkVersionFlashFonts()) {
      Error_Handler();
   }

   transport_t transport = ask_transport(); // S7 = USB-CDC, S6 = UART-via-USB
   init_input(transport);                   // Handshake H/R/G mit dem PC

   show_s5_hint();
   wait_for_s_i(BUTTON_S5); // erst S5 startet das erste Bild

   while (1) {
      /* Rueckgabewert bewusst ignoriert: Ein Fehler steht bereits auf dem
         LCD, in beiden Faellen geht es erst mit S5 weiter. */
      (void)handle_picture();
      wait_for_s_i(BUTTON_S5); // S5 = naechstes Bild
   }
}
// EOF
