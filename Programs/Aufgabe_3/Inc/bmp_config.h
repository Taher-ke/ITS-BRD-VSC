/**
 ******************************************************************************
 * @file    bmp_config.h
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Zentrale Konstanten und Schalter der BITMAP-Anwendung.
 *
 *          Mit den Schaltern lassen sich die drei Teilaufgaben vorfuehren:
 *            Teilaufgabe a: USE_LINE_OUTPUT = 0, USE_SCALING = 0
 *            Teilaufgabe b: USE_LINE_OUTPUT = 1, USE_SCALING = 0
 *            Teilaufgabe c: USE_LINE_OUTPUT = 1, USE_SCALING = 1 (Abgabe)
 ******************************************************************************
 */

#ifndef _BMP_CONFIG_H
#define _BMP_CONFIG_H

/* ---------------------------------------------------------------------------
 * Display (Waveshare-LCD des ITS-Boards, Querformat)
 * ------------------------------------------------------------------------- */
#define DISPLAY_WIDTH 480  /**< Anzahl Spalten des LCD */
#define DISPLAY_HEIGHT 320 /**< Anzahl Zeilen des LCD */

/* ---------------------------------------------------------------------------
 * Groessengrenzen (Teilaufgabe c: hoechstens auf 1/5 verkleinern)
 * ------------------------------------------------------------------------- */
#define MAX_SCALE_DOWN 5                                  /**< max. Verkleinerung */
#define MAX_IMG_WIDTH (MAX_SCALE_DOWN * DISPLAY_WIDTH)    /**< 2400 Pixel */
#define MAX_IMG_HEIGHT (MAX_SCALE_DOWN * DISPLAY_HEIGHT)  /**< 1600 Pixel */

/* ---------------------------------------------------------------------------
 * Schalter
 * ------------------------------------------------------------------------- */
/** 0: jedes Pixel mit GUI_drawPoint (Teilaufgabe a)
 *  1: ganze Zeile mit GUI_WriteLine (Teilaufgabe b, schneller) */
#define USE_LINE_OUTPUT 1

/** 0: grosse Bilder werden am Displayrand abgeschnitten (Teilaufgabe a/b)
 *  1: grosse Bilder werden mit dem Box-Algorithmus verkleinert (c) */
#define USE_SCALING 1

/** 1: vor jedem Bild beide Header anzeigen (input_file_header(), dann S5)
 *  0: Bild direkt anzeigen */
#define SHOW_HEADERS 0

/** 1: nach dem Zeichnen die Dauer in ms unten links ausgeben
 *  (zum Vergleich Teilaufgabe a gegen b) */
#define SHOW_DRAW_TIME 0

/** Timer-Ticks pro Millisekunde nach switch_to_168_MHz() (84 Ticks/us) */
#define TICKS_PER_MS 84000U

#endif /* _BMP_CONFIG_H */
// EOF
