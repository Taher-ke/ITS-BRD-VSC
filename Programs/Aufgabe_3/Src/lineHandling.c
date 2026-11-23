/**
 ******************************************************************************
 * @file    lineHandling.c
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Implementierung von lineHandling.h (Zeilenpuffer + Box-Algorithmus).
 *
 *          Abbildung Bild -> Display (je Achse):
 *            Displayposition = floor(Bildposition * n_out / n_in)
 *          mit n_in = Bildgroesse und n_out = Groesse auf dem Display. Alle
 *          Bildpositionen mit derselben Displayposition bilden eine Kachel.
 *          Ohne Verkleinerung ist n_out == n_in, jede Kachel ist 1 Pixel gross.
 *
 *          Speicherbedarf: 3 x 480 Summen + 480 Farbwerte (~6,7 KB, static).
 *          Das ist weniger als 5 gepufferte Bildzeilen (5 x 2400 x 3 Byte).
 ******************************************************************************
 */

#include "lineHandling.h"
#include "bmp_config.h"
#include "lcdOutput.h"
#include <stdbool.h>
#include <string.h>

/* Summen der Farbanteile je Display-Spalte fuer die aktuelle Kachelzeile.
   static: liegen nicht auf dem Stack und sind nur hier sichtbar. */
static uint32_t sum_red[DISPLAY_WIDTH];
static uint32_t sum_green[DISPLAY_WIDTH];
static uint32_t sum_blue[DISPLAY_WIDTH];
static COLOR line_buf[DISPLAY_WIDTH]; /**< fertige LCD-Zeile (RGB565) */

static int32_t in_width = 1;   /**< Bildbreite */
static int32_t in_height = 1;  /**< Bildhoehe */
static int32_t out_width = 1;  /**< Breite auf dem Display (vor dem Abschneiden) */
static int32_t out_height = 1; /**< Hoehe auf dem Display (vor dem Abschneiden) */
static int32_t row = 0;        /**< aktuelle Bildzeile, 0 = unterste */
static int32_t out_y = -1;     /**< Display-Zeile der aktuellen Bildzeile, -1 = keine */
static bool dirty = false;     /**< Summen enthalten noch nicht ausgegebene Werte */

/**
 * @brief  Bildposition -> Displayposition: floor(pos * n_out / n_in).
 */
static int32_t map_pos(int32_t pos, int32_t n_in, int32_t n_out) {
   return (int32_t)(((uint32_t)pos * (uint32_t)n_out) / (uint32_t)n_in);
}

/**
 * @brief  Erste Bildposition der Kachel bin: ceil(bin * n_in / n_out).
 */
static int32_t bin_start(int32_t bin, int32_t n_in, int32_t n_out) {
   return (int32_t)((((uint32_t)bin * (uint32_t)n_in) + (uint32_t)n_out - 1U) / (uint32_t)n_out);
}

/**
 * @brief  Anzahl der Bildpositionen in der Kachel bin (>= 1, da n_out <= n_in).
 */
static int32_t bin_size(int32_t bin, int32_t n_in, int32_t n_out) {
   return bin_start(bin + 1, n_in, n_out) - bin_start(bin, n_in, n_out);
}

/**
 * @brief  Display-Zeile einer Bildzeile. BMP ist bottom-up: Bildzeile 0 ist
 *         die unterste, also von oben gezaehlt in_height - 1 - r.
 * @retval Display-Zeile oder -1, wenn r hinter der letzten Bildzeile liegt
 */
static int32_t out_row_of(int32_t r) {
   if (r >= in_height) {
      return -1;
   }
   return map_pos(in_height - 1 - r, in_height, out_height);
}

/**
 * @brief  Bildet die Mittelwerte der aktuellen Kachelzeile, gibt sie als eine
 *         LCD-Zeile aus und setzt die Summen zurueck.
 */
static void flush_line(void) {
   if (!dirty) {
      return; /* nichts gesammelt (z. B. abgeschnittene Zeile) */
   }
   dirty = false;
   int32_t len = (out_width < DISPLAY_WIDTH) ? out_width : DISPLAY_WIDTH;
   uint32_t rows = (uint32_t)bin_size(out_y, in_height, out_height);
   for (int32_t x = 0; x < len; x++) {
      /* Pixel der Kachel = Zeilen x Spalten; nicht gelieferte Pixel
         (Delta / Zeile zu kurz) zaehlen als schwarz mit */
      uint32_t n = rows * (uint32_t)bin_size(x, in_width, out_width);
      uint32_t half = n / 2U; /* runden statt abschneiden */
      line_buf[x] = rgb_to_lcd((uint8_t)((sum_red[x] + half) / n),
                               (uint8_t)((sum_green[x] + half) / n),
                               (uint8_t)((sum_blue[x] + half) / n));
      sum_red[x] = 0U;
      sum_green[x] = 0U;
      sum_blue[x] = 0U;
   }
   lcd_write_line(out_y, line_buf, len);
}

void lh_begin(int32_t width, int32_t height) {
   in_width = width;
   in_height = height;
#if USE_SCALING
   if ((width <= DISPLAY_WIDTH) && (height <= DISPLAY_HEIGHT)) {
      out_width = width; /* passt: nicht verkleinern */
      out_height = height;
   } else if (((uint32_t)width * DISPLAY_HEIGHT) >= ((uint32_t)height * DISPLAY_WIDTH)) {
      out_width = DISPLAY_WIDTH; /* relativ breiter als das Display */
      out_height = (int32_t)(((uint32_t)height * DISPLAY_WIDTH) / (uint32_t)width);
   } else {
      out_height = DISPLAY_HEIGHT; /* relativ hoeher als das Display */
      out_width = (int32_t)(((uint32_t)width * DISPLAY_HEIGHT) / (uint32_t)height);
   }
   /* gleicher Faktor fuer x und y -> keine Verzerrung; mind. 1 Pixel */
   out_width = (out_width < 1) ? 1 : out_width;
   out_height = (out_height < 1) ? 1 : out_height;
#else
   out_width = width; /* nicht verkleinern, Rest wird abgeschnitten */
   out_height = height;
#endif
   (void)memset(sum_red, 0, sizeof(sum_red));
   (void)memset(sum_green, 0, sizeof(sum_green));
   (void)memset(sum_blue, 0, sizeof(sum_blue));
   row = 0;
   out_y = out_row_of(row);
   dirty = false;
}

void lh_put_run(int32_t x, int32_t count, RGBTRIPLE rgb) {
   if ((out_y < 0) || (out_y >= DISPLAY_HEIGHT)) {
      return; /* Zeile liegt ausserhalb des Displays */
   }
   for (int32_t i = x; i < (x + count); i++) {
      int32_t out_x = (out_width == in_width) ? i : map_pos(i, in_width, out_width);
      if (out_x >= DISPLAY_WIDTH) {
         break; /* Abbildung ist monoton: alle weiteren auch ausserhalb */
      }
      sum_red[out_x] += rgb.rgbtRed;
      sum_green[out_x] += rgb.rgbtGreen;
      sum_blue[out_x] += rgb.rgbtBlue;
   }
   dirty = true;
}

void lh_next_rows(int32_t n) {
   row += n;
   int32_t new_out_y = out_row_of(row);
   if (new_out_y != out_y) {
      flush_line(); /* Kachelzeile vollstaendig -> ausgeben */
      out_y = new_out_y;
   }
}

void lh_finish(void) {
   flush_line(); /* letzte, evtl. unvollstaendige Kachelzeile */
}
// EOF
