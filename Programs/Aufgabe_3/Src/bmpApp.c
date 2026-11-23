/**
 ******************************************************************************
 * @file    bmpApp.c
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Implementierung von bmpApp.h.
 ******************************************************************************
 */

#include "bmpApp.h"
#include "BMP_types.h"
#include "LCD_GUI.h"
#include "bmp_config.h"
#include "colorTable.h"
#include "compressedBMP.h"
#include "errorhandler.h"
#include "fileHeader.h"
#include "headers.h"
#include "input.h"
#include "lineHandling.h"
#include "uncompressedBMP.h"
#include <stdint.h>

#if SHOW_DRAW_TIME
#include "lcd.h"
#include "timer.h"
#include <stdio.h>
#define TIME_ROW 19      /* unterste Textzeile des LCD */
#define TIME_BUF_SIZE 24
#endif

/**
 * @brief  Oeffnet die naechste Datei und liest + prueft beide Header.
 */
static int open_and_read_headers(void) {
#if SHOW_HEADERS
   return input_file_header(); /* oeffnen, pruefen, anzeigen, auf S5 warten */
#else
   open_nxt_file();       /* naechste Datei vom PC anfordern */
   GUI_clear(BLACK);      /* altes Bild loeschen */
   return read_hdrs();    /* Header lesen + pruefen (Modul headers) */
#endif
}

/**
 * @brief  Waehlt den passenden Dekoder fuer das Format des Bilds.
 */
static int decode_pixels(const BITMAPINFOHEADER *info_hdr_p) {
   if ((8 == info_hdr_p->biBitCount) && (BI_RLE8 == info_hdr_p->biCompression)) {
      return decode_rle8(info_hdr_p->biWidth, info_hdr_p->biHeight);
   }
   if (BI_RGB == info_hdr_p->biCompression) {
      return decode_uncompressed(info_hdr_p->biWidth, info_hdr_p->biHeight, info_hdr_p->biBitCount);
   }
   return CHECK(false, UNSUPPORTED_FORMAT_ERR); // andere Kodierungen
}

int handle_picture(void) {
   BITMAPFILEHEADER file_hdr;
   BITMAPINFOHEADER info_hdr;

   RETURN_ON_ERROR(open_and_read_headers());
   RETURN_ON_ERROR(get_file_hdr(&file_hdr));
   RETURN_ON_ERROR(get_info_hdr(&info_hdr));

   /* Teilaufgabe c: hoechstens auf 1/5 verkleinern */
   CHECK_OR_RETURN((info_hdr.biWidth <= MAX_IMG_WIDTH) && (info_hdr.biHeight <= MAX_IMG_HEIGHT),
                   IMG_SIZE_ERR); // Bild groesser als 5 x Display

   RETURN_ON_ERROR(ct_read(&file_hdr, &info_hdr)); /* Farbtabelle, bis bfOffBits */

#if SHOW_DRAW_TIME
   uint32_t start = getTimeStamp();
#endif
   lh_begin(info_hdr.biWidth, info_hdr.biHeight);
   RETURN_ON_ERROR(decode_pixels(&info_hdr));
   lh_finish(); /* letzte Zeile ausgeben */

#if SHOW_DRAW_TIME
   char buf[TIME_BUF_SIZE];
   (void)snprintf(buf, sizeof(buf), "%lu ms", (unsigned long)((getTimeStamp() - start) / TICKS_PER_MS));
   lcdGotoXY(1, TIME_ROW);
   lcdPrintS(buf);
#endif
   return EOK;
}
// EOF
