/**
 ******************************************************************************
 * @file    colorTable.c
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Implementierung von colorTable.h.
 ******************************************************************************
 */

#include "colorTable.h"
#include "errorhandler.h"
#include "input.h"
#include <stdint.h>
#include <stdio.h> /* EOF */

/* Groesse beider Header = Position der Farbtabelle in der Datei (54 Byte) */
#define HEADERS_SIZE (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))

/* Information Hiding: Tabelle nur in diesem Modul sichtbar.
   static statt lokal: 1 KB wuerde sonst den Stack belasten. */
static RGBQUAD color_table[MAX_COLOR_TABLE_SIZE];
static int num_colors = 0; /**< Anzahl gueltiger Eintraege */

int ct_read(const BITMAPFILEHEADER *file_hdr_p, const BITMAPINFOHEADER *info_hdr_p) {
   num_colors = 0;
   if (8 == info_hdr_p->biBitCount) {
      num_colors = (0 == info_hdr_p->biClrUsed) ? MAX_COLOR_TABLE_SIZE : (int)info_hdr_p->biClrUsed;
      CHECK_OR_RETURN(num_colors == read_file((char *)color_table, sizeof(RGBQUAD), (unsigned int)num_colors),
                      COL_TAB_ERR); // Farbtabelle unvollstaendig
   }

   /* Bis zum Beginn der Pixeldaten (bfOffBits) springen */
   uint32_t pos = (uint32_t)HEADERS_SIZE + ((uint32_t)num_colors * (uint32_t)sizeof(RGBQUAD));
   CHECK_OR_RETURN(pos <= file_hdr_p->bfOffBits, COL_TAB_ERR); // Tabelle ragt in die Pixeldaten
   for (; pos < file_hdr_p->bfOffBits; pos++) {
      CHECK_OR_RETURN(EOF != nxt_chr(), READ_ERR); // Datei endet vor den Pixeldaten
   }
   return EOK;
}

int ct_lookup(int idx, RGBTRIPLE *rgb_p) {
   CHECK_OR_RETURN((0 <= idx) && (idx < num_colors), COL_TAB_ERR); // Index ausserhalb der Tabelle
   rgb_p->rgbtBlue = color_table[idx].rgbBlue;
   rgb_p->rgbtGreen = color_table[idx].rgbGreen;
   rgb_p->rgbtRed = color_table[idx].rgbRed;
   return EOK;
}
// EOF
