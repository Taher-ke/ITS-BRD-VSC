/**
 ******************************************************************************
 * @file    uncompressedBMP.h
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Dekoder fuer nicht komprimierte BMP (BI_RGB) mit 8 Bit pro Pixel
 *          (Farbtabelle) oder 24 Bit pro Pixel (RGBTRIPLE).
 ******************************************************************************
 */

#ifndef _UNCOMPRESSED_BMP_H
#define _UNCOMPRESSED_BMP_H

#include <stdint.h>

/**
 * @brief  Dekodiert die Pixeldaten eines nicht komprimierten Bilds. Jede
 *         Zeile ist auf ein Vielfaches von 4 Byte aufgefuellt (Padding).
 *         Der Lesezeiger steht am Anfang der Pixeldaten, lh_begin() ist
 *         gerufen.
 * @param  width      Bildbreite in Pixeln
 * @param  height     Bildhoehe in Pixeln
 * @param  bit_count  8 oder 24 Bit pro Pixel
 * @retval EOK, COL_TAB_ERR, READ_ERR oder UNSUPPORTED_FORMAT_ERR
 */
extern int decode_uncompressed(int32_t width, int32_t height, int bit_count);

#endif /* _UNCOMPRESSED_BMP_H */
// EOF
