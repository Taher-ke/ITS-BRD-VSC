/**
 ******************************************************************************
 * @file    compressedBMP.c
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Implementierung von compressedBMP.h (RLE8-Dekoder).
 *
 *          Aufbau der Daten (immer 2-Byte-Paare):
 *            n  c          Encoded Mode: n Pixel (n >= 1) mit Farbindex c
 *            00 00         End of line
 *            00 01         End of bitmap
 *            00 02 dx dy   Delta: dx Spalten nach rechts, dy Zeilen weiter
 *            00 n  ...     Absolute Mode (n = 3..255): n Farbindizes folgen,
 *                          bei ungeradem n ein Fuellbyte (16-Bit-Grenze)
 ******************************************************************************
 */

#include "compressedBMP.h"
#include "colorTable.h"
#include "errorhandler.h"
#include "input.h"
#include "lineHandling.h"
#include <stdio.h> /* EOF */

/* Werte des zweiten Bytes nach dem Escape-Byte 0 */
#define RLE_ESCAPE 0 /* erstes Byte 0: Escape bzw. Absolute Mode */
#define RLE_EOL 0    /* End of line */
#define RLE_EOB 1    /* End of bitmap */
#define RLE_DELTA 2  /* Delta */

/* Zustand des Dekoders fuer das aktuelle Bild */
static int32_t img_width;  /**< Bildbreite */
static int32_t img_height; /**< Bildhoehe */
static int32_t x;          /**< naechste Spalte in der aktuellen Zeile */
static int32_t row;        /**< aktuelle Zeile, 0 = unterste */

/**
 * @brief  Liest das naechste Byte der Datei.
 * @param  byte_p  Ausgabe: gelesenes Byte 0..255
 * @retval EOK oder READ_ERR, wenn die Datei vorzeitig endet
 */
static int read_byte(int *byte_p) {
   int chr = nxt_chr();
   CHECK_OR_RETURN(EOF != chr, READ_ERR); // Datei endet vor "End of bitmap"
   *byte_p = chr;
   return EOK;
}

/**
 * @brief  Prueft, ob count weitere Pixel in die aktuelle Zeile passen.
 * @retval EOK oder FORMAT_ERR
 */
static int check_room(int32_t count) {
   CHECK_OR_RETURN(row < img_height, FORMAT_ERR);         // mehr Zeilen als biHeight
   CHECK_OR_RETURN(x + count <= img_width, FORMAT_ERR);   // Zeile laenger als biWidth
   return EOK;
}

/**
 * @brief  Encoded Mode: count Pixel mit demselben Farbindex.
 */
static int encoded_run(int32_t count, int color_idx) {
   RGBTRIPLE rgb;
   RETURN_ON_ERROR(check_room(count));
   RETURN_ON_ERROR(ct_lookup(color_idx, &rgb));
   lh_put_run(x, count, rgb);
   x += count;
   return EOK;
}

/**
 * @brief  Absolute Mode: count einzelne Farbindizes, danach ggf. ein
 *         Fuellbyte, damit der Lauf auf einer 16-Bit-Grenze endet.
 */
static int absolute_run(int32_t count) {
   RETURN_ON_ERROR(check_room(count));
   for (int32_t i = 0; i < count; i++) {
      int color_idx;
      RGBTRIPLE rgb;
      RETURN_ON_ERROR(read_byte(&color_idx));
      RETURN_ON_ERROR(ct_lookup(color_idx, &rgb));
      lh_put_run(x, 1, rgb);
      x++;
   }
   if (0 != (count & 1)) {
      int pad;
      RETURN_ON_ERROR(read_byte(&pad)); /* Fuellbyte ueberlesen */
   }
   return EOK;
}

/**
 * @brief  End of line: naechste Zeile, wieder bei Spalte 0.
 */
static int end_of_line(void) {
   CHECK_OR_RETURN(row < img_height, FORMAT_ERR); // mehr Zeilen als biHeight
   lh_next_rows(1);
   row++;
   x = 0;
   return EOK;
}

/**
 * @brief  Delta: dx Spalten nach rechts und dy Zeilen weiter. Die
 *         uebersprungenen Pixel bleiben schwarz.
 */
static int delta(void) {
   int dx;
   int dy;
   RETURN_ON_ERROR(read_byte(&dx));
   RETURN_ON_ERROR(read_byte(&dy));
   CHECK_OR_RETURN(x + dx <= img_width, FORMAT_ERR);   // Sprung hinter das Zeilenende
   CHECK_OR_RETURN(row + dy <= img_height, FORMAT_ERR); // Sprung hinter die letzte Zeile
   if (0 < dy) {
      lh_next_rows(dy);
      row += dy;
   }
   x += dx;
   return EOK;
}

int decode_rle8(int32_t width, int32_t height) {
   img_width = width;
   img_height = height;
   x = 0;
   row = 0;
   for (;;) {
      int first;
      int second;
      RETURN_ON_ERROR(read_byte(&first));
      RETURN_ON_ERROR(read_byte(&second));
      if (RLE_ESCAPE != first) {
         RETURN_ON_ERROR(encoded_run(first, second)); /* n Pixel, Farbe c */
      } else if (RLE_EOL == second) {
         RETURN_ON_ERROR(end_of_line());
      } else if (RLE_EOB == second) {
         return EOK; /* Bild vollstaendig */
      } else if (RLE_DELTA == second) {
         RETURN_ON_ERROR(delta());
      } else {
         RETURN_ON_ERROR(absolute_run(second)); /* 3..255 Einzelpixel */
      }
   }
}
// EOF
