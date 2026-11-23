/**
 ******************************************************************************
 * @file    lcdOutput.c
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Implementierung von lcdOutput.h.
 ******************************************************************************
 */

#include "lcdOutput.h"
#include "bmp_config.h"

/* RGB565: von 8 Bit pro Farbanteil bleiben 5 (rot), 6 (gruen), 5 (blau) */
#define RED_SHIFT_IN 3    /* 8 -> 5 Bit */
#define GREEN_SHIFT_IN 2  /* 8 -> 6 Bit */
#define BLUE_SHIFT_IN 3   /* 8 -> 5 Bit */
#define RED_POS 11        /* rot liegt in Bit 15..11 */
#define GREEN_POS 5       /* gruen liegt in Bit 10..5 */

COLOR rgb_to_lcd(uint8_t red, uint8_t green, uint8_t blue) {
   return (COLOR)(((uint16_t)(red >> RED_SHIFT_IN) << RED_POS) |
                  ((uint16_t)(green >> GREEN_SHIFT_IN) << GREEN_POS) |
                  (uint16_t)(blue >> BLUE_SHIFT_IN));
}

void lcd_write_line(int32_t y, COLOR *line, int32_t len) {
#if USE_LINE_OUTPUT
   /* Teilaufgabe b: Koordinate einmal, dann alle Farbwerte als Burst */
   Coordinate start = {0, (uint16_t)y};
   GUI_WriteLine(start, (unsigned int)len, line);
#else
   /* Teilaufgabe a: fuer jedes Pixel Koordinaten + Farbe uebertragen */
   for (int32_t x = 0; x < len; x++) {
      GUI_drawPoint((POINT)x, (POINT)y, line[x], DOT_PIXEL_1X1, DOT_FILL_AROUND);
   }
#endif
}
// EOF
