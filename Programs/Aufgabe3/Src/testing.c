/**
 ******************************************************************************
 * @file    fileHeader.c
 * @brief   Gekapselte Einstiegsfunktion der Input-Verarbeitung
 *          (Implementierung von fileHeader.h).
 *
 *          input_file_header() wird ganz am Anfang der Behandlung eines
 *          Bilds aufgerufen und fuehrt folgende Aufgaben aus:
 *            1. naechste Datei vom PC oeffnen       (open_nxt_file, input.h)
 *            2. BITMAP-Header einlesen und testen   (read_hdrs, headers.h)
 *            3. beide Header (BITMAPFILEHEADER + BITMAPINFOHEADER) auf dem
 *               LCD-Display ausgeben; danach ohne Textausgabe auf S5 (PF5)
 *               warten (wait_for_s_i), dann Display loeschen -- erst dann
 *               folgt die Darstellung des Bildes
 ******************************************************************************
 */

#include "BMP_types.h"
#include "LCD_general.h"
#include "errorhandler.h"
#include "headers.h"
#include "input.h"
#include "LCD_GUI.h"
#include "lcd.h"
#include <stdint.h>
#include <stdio.h>
#include <limits.h>

/* Byte pro Pixel bei Verwendung des RGB-Formats */
#define RGB_BYTE_COUNT 3
/* Byte pro Pixel bei Verwendung der Colour Palette */
#define PALETTE_POINTER_BYTE_COUNT 1
/* Byte eines Paletten-Pixels */
#define PALETTE_COLOR_BYTE_COUNT 4
/* char size in Bytes*/
#define CHAR_SIZE 1
/* Länge der Palette in Bytes */
#define PALETTE_SIZE 256
/* Anzahl Bytes in einem Pixel abhängig vom Format */
#define PIXEL_BYTE_COUNT(uses_palette) \
    ((uses_palette) ? PALETTE_POINTER_BYTE_COUNT : RGB_BYTE_COUNT)

#define SHIFT_TO_5_BIT 3
#define SHIFT_TO_6_BIT 2
#define LEFT_SHIFT_GREEN 5
#define LEFT_SHIFT_RED 11

static int32_t pic_width;
static int32_t pic_height;
static bool uses_palette;
static RGBQUAD palette[PALETTE_SIZE];


int read_pixel(RGBTRIPLE *pixel) {
    // nächsten Pixel lesen
    char pixel_buf[PIXEL_BYTE_COUNT(uses_palette)];
    if (EOF == read_file(pixel_buf, CHAR_SIZE, PIXEL_BYTE_COUNT(uses_palette))) {
        return EOF;
    }
    // falls 8 bit: in 24 Bit umwandeln
    if (uses_palette) {
        // uint32_t palette_bit_offset = file_hdr.bfOffBits;
        RGBQUAD *color = &palette[(uint8_t) pixel_buf[0]];
        pixel->rgbtBlue  = color->rgbBlue;
        pixel->rgbtGreen = color->rgbGreen;
        pixel->rgbtRed   = color->rgbRed;
    }
    else {
        pixel->rgbtBlue  = pixel_buf[0];
        pixel->rgbtGreen = pixel_buf[1];
        pixel->rgbtRed   = pixel_buf[2];
    }
    return EOK;
}

int read_palette(void) {
    RGBQUAD palette_color;
    char palette_color_buf[PALETTE_COLOR_BYTE_COUNT];
    // Für alle 256 Farben
    for (int i = 0; i < PALETTE_SIZE; i++) {
        // Ein RGBQUAD einlesen
        if (EOF == read_file(palette_color_buf, CHAR_SIZE, PALETTE_COLOR_BYTE_COUNT)) {
            return EOF;
        }
        palette_color.rgbBlue  = palette_color_buf[0];
        palette_color.rgbGreen = palette_color_buf[1];
        palette_color.rgbRed   = palette_color_buf[2];
        palette_color.rgbReserved   = palette_color_buf[3];
        palette[i] = palette_color;
    }
    return EOK;
}

uint16_t pixel_to_color(RGBTRIPLE *pixel) {
    return (
        //blue
        (((uint16_t) pixel->rgbtBlue) >> SHIFT_TO_5_BIT) |
        //green
        ((((uint16_t) pixel->rgbtGreen) >> SHIFT_TO_6_BIT) << LEFT_SHIFT_GREEN) |
        //red
        ((((uint16_t) pixel->rgbtRed) >> SHIFT_TO_5_BIT) << LEFT_SHIFT_RED)
        );
}

int print_pixel(RGBTRIPLE *pixel, Coordinate *crd) {
    // 3. Einen Pixel einlesen
    RETURN_ON_ERROR(read_pixel(pixel));
    // pixel in lcd-format umwandeln
    uint16_t color = pixel_to_color(pixel);
    // pixel ausgeben
    GUI_drawPoint(*crd, color, DOT_PIXEL_1X1, DOT_FILL_AROUND);
    // Koordinaten erhöhen
    if (crd->x == pic_width - 1) {
        if (crd->y == pic_height - 1){
            return EOF;
        }
        crd->x = 0;
        crd->y++;
    }
    else {
        crd->x++;
    }
    return EOK;
}

int print_picture(void) {
    // 1. naechste Datei anfordern und Header holen
    open_nxt_file(); /* naechste Datei vom PC anfordern */
    RETURN_ON_ERROR(read_hdrs()); /* Header einlesen + testen (Modul headers) */
    BITMAPFILEHEADER file_hdr;
    BITMAPINFOHEADER info_hdr;
    RETURN_ON_ERROR(get_file_hdr(&file_hdr)); /* ersten Header holen */
    RETURN_ON_ERROR(get_info_hdr(&info_hdr)); /* zweiten Header holen */

    // 2. Header prüfen/auswerten
    // bild breite und höhe
    pic_width = info_hdr.biWidth;
    pic_height = info_hdr.biHeight;
    // 24 bit oder 8 bit?
    uses_palette = (PALETTE_POINTER_BYTE_COUNT == info_hdr.biBitCount / CHAR_BIT);
    // TODO ist compressed? (später)!!!!!!!!!!!!!!!!!!!!!
    // palette einlesen, wenn 8 bit format (array mit palette colors füllen)
    if (uses_palette) {
        RETURN_ON_ERROR(read_palette());      
    }

    // Display leeren
    GUI_clear(WHITE);
    Coordinate crd = {0, 0};
    RGBTRIPLE pixel;

    while(true) {
        RETURN_ON_ERROR(print_pixel(&pixel, &crd));
    }
    return EOK;
}
// CHECK_FATAL(!uses_palette, NOT_IMPLEMENTED_ERR);


// EOF
