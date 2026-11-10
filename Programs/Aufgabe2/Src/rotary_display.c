/**
 * @file    rotary_display.c
 * @author  Taher Keshavarzian, Sönke Wulf
 * @date    09.11.2026
 * @brief   Zeichenweise LCD-Ausgabe von Winkel und Winkelgeschwindigkeit.
 */

#include "rotary_display.h"
#include "LCD_GUI.h"
#include "lcd.h"
#include <stdio.h>

/* Feste Texte, nur einmal ausgegeben. Die Werte stehen ab Spalte 19. */
#define ANGLE_TXT "Winkel          :  0.0          Grad"
#define SPEED_TXT "Geschwindigkeit :  0.0          Grad/s"

#define YPOS_MIDDLE (19 / 2)
#define XPOS_TXT 2
#define XPOS_VAL 19
#define YPOS_ANGLE_TXT (YPOS_MIDDLE - 2)
#define YPOS_SPEED_TXT (YPOS_MIDDLE + 2)

#define VAL_WIDTH 9              /**< Zeichen pro Wert ("%9.1f") */
#define BUF_SIZE (VAL_WIDTH + 1) /**< + Nullterminator */
#define NUM_POS (2 * VAL_WIDTH)  /**< Positionen pro Ausgabezyklus */

/* Information Hiding: nur in dieser Datei sichtbar */
static char buf_angle[BUF_SIZE];
static char buf_angle_shown[BUF_SIZE]; /**< aktuell auf dem LCD sichtbar */
static char buf_speed[BUF_SIZE];
static char buf_speed_shown[BUF_SIZE];
static int pos = 0; /**< 0..8 Winkel, 9..17 Geschwindigkeit */

void init_display(void) {
  GUI_init(DEFAULT_BRIGHTNESS);

  lcdGotoXY(XPOS_TXT, YPOS_ANGLE_TXT);
  lcdPrintS(ANGLE_TXT);
  lcdGotoXY(XPOS_TXT, YPOS_SPEED_TXT);
  lcdPrintS(SPEED_TXT);
}

/**
 * @brief Schreibt ein Zeichen, wenn es sich von der Anzeige unterscheidet.
 */
static void print_if_changed(char *buf, char *shown, int idx, int ypos) {
  if (buf[idx] != shown[idx]) {
    lcdGotoXY(XPOS_VAL + idx, ypos);
    lcdPrintC(buf[idx]);
    shown[idx] = buf[idx];
  }
}

void update_display(double angle, double angle_per_sec) {
  if (pos == 0) {
    /* Snapshot: alle Zeichen eines Zyklus stammen von denselben Werten. */
    (void)snprintf(buf_angle, BUF_SIZE, "%9.1f", angle);
    (void)snprintf(buf_speed, BUF_SIZE, "%9.1f", angle_per_sec);
  }

  if (pos < VAL_WIDTH) {
    print_if_changed(buf_angle, buf_angle_shown, pos, YPOS_ANGLE_TXT);
  } else {
    print_if_changed(buf_speed, buf_speed_shown, pos - VAL_WIDTH, YPOS_SPEED_TXT);
  }

  pos++;
  if (pos >= NUM_POS) {
    pos = 0;
  }
}
// EOF
