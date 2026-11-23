/**
 ******************************************************************************
 * @file    uncompressedBMP.c
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Implementierung von uncompressedBMP.h.
 ******************************************************************************
 */

#include "uncompressedBMP.h"
#include "BMP_types.h"
#include "colorTable.h"
#include "errorhandler.h"
#include "input.h"
#include "lineHandling.h"
#include <stdio.h> /* EOF */

#define BITS_8 8   /* 8 Bit pro Pixel: Index in die Farbtabelle */
#define BITS_24 24 /* 24 Bit pro Pixel: RGBTRIPLE (blau, gruen, rot) */
#define BITS_PER_BYTE 8U

/**
 * @brief  Bytes pro Zeile inkl. Padding: (((Breite * Bits) + 31) / 32) * 4.
 *         Eine Zeile belegt immer ein Vielfaches von 4 Byte (32 Bit).
 */
static uint32_t row_size(int32_t width, int bit_count) {
   return ((((uint32_t)width * (uint32_t)bit_count) + 31U) / 32U) * 4U;
}

/**
 * @brief  Liest das naechste Pixel und liefert seine Farbe.
 * @param  bit_count  8 oder 24
 * @param  rgb_p      Ausgabe: Farbe des Pixels
 * @retval EOK, COL_TAB_ERR oder READ_ERR
 */
static int read_pixel(int bit_count, RGBTRIPLE *rgb_p) {
   if (BITS_8 == bit_count) {
      int color_idx = nxt_chr();
      CHECK_OR_RETURN(EOF != color_idx, READ_ERR); // Datei zu kurz
      return ct_lookup(color_idx, rgb_p);
   }
   /* 24 Bit: die Bytes liegen in der Reihenfolge des RGBTRIPLE */
   CHECK_OR_RETURN(1 == read_file((char *)rgb_p, sizeof(RGBTRIPLE), 1), READ_ERR); // Datei zu kurz
   return EOK;
}

int decode_uncompressed(int32_t width, int32_t height, int bit_count) {
   CHECK_OR_RETURN((BITS_8 == bit_count) || (BITS_24 == bit_count), UNSUPPORTED_FORMAT_ERR);
   uint32_t used_bytes = (uint32_t)width * ((uint32_t)bit_count / BITS_PER_BYTE);
   uint32_t pad_bytes = row_size(width, bit_count) - used_bytes;

   for (int32_t row = 0; row < height; row++) {
      for (int32_t x = 0; x < width; x++) {
         RGBTRIPLE rgb;
         RETURN_ON_ERROR(read_pixel(bit_count, &rgb));
         lh_put_run(x, 1, rgb);
      }
      for (uint32_t i = 0; i < pad_bytes; i++) {
         CHECK_OR_RETURN(EOF != nxt_chr(), READ_ERR); // Padding-Bytes ueberlesen
      }
      lh_next_rows(1);
   }
   return EOK;
}
// EOF
