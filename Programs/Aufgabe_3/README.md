# GS Praktikumsaufgabe 3 – BITMAP


Das Programm empfängt BMP-Dateien über USB, dekodiert sie, verkleinert große Bilder mit einem Box-Algorithmus und zeigt sie auf dem LCD (480 × 320) an.

## 1. Modulkonzept

Datenfluss:

```
PC (Python-GUI) ──USB──> input (vorgegeben) ──> headers (vorgegeben)
                                   │
main ──> bmpApp ──> colorTable     │  Farbtabelle lesen, bis bfOffBits springen
            │                      │
            ├──> compressedBMP ────┤  RLE8 dekodieren           ┐
            └──> uncompressedBMP ──┘  BI_RGB 8/24 Bit dekodieren┘──> lineHandling ──> lcdOutput ──> LCD
                                                                    (Zeilen sammeln,   (RGB565,
                                                                     Box-Algorithmus)   WriteLine/drawPoint)
```

| Modul | Aufgabe | Schnittstelle |
|---|---|---|
| `main.c` | Initialisierung, Transport wählen, Endlosschleife: Bild zeigen, auf S5 warten | – |
| `bmpApp` | Ablauf pro Bild: Datei öffnen, Header prüfen, Größe prüfen, Farbtabelle, passenden Dekoder aufrufen | `handle_picture()` |
| `colorTable` | Farbtabelle lesen (biClrUsed bzw. 256 Einträge), bis `bfOffBits` überlesen, Index → Farbe | `ct_read()`, `ct_lookup()` |
| `compressedBMP` | RLE8-Dekoder: Encoded Mode, Absolute Mode, End of line, End of bitmap, Delta | `decode_rle8()` |
| `uncompressedBMP` | Dekoder für BI_RGB mit 8 Bit (Palette) und 24 Bit (RGBTRIPLE), Padding auf 4 Byte | `decode_uncompressed()` |
| `lineHandling` | Pixel zeilenweise sammeln, bottom-up → top-down, Box-Mittelwert für große Bilder, fertige Zeile ausgeben | `lh_begin()`, `lh_put_run()`, `lh_next_rows()`, `lh_finish()` |
| `lcdOutput` | Einzige Stelle mit LCD-Pixelausgabe: RGB888 → RGB565, `GUI_WriteLine` (b) oder `GUI_drawPoint` (a) | `rgb_to_lcd()`, `lcd_write_line()` |
| `bmp_config.h` | Displaygröße, Grenzen (5 × Display), Schalter für Teilaufgabe a/b/c | – |
| vorgegeben | `input`, `headers`, `fileHeader`, `errorhandler`, `BMP_types`, USB-Module | unverändert |

Die Dekoder kennen das Display nicht, `lineHandling` kennt das Dateiformat nicht. Jedes Modul hält seinen Zustand in `static`-Variablen (Information Hiding). Große Puffer sind `static` und liegen nicht auf dem Stack.

## 2. Fehlerbehandlung

- Alle Prüfungen nutzen die vorgegebenen Makros `CHECK_OR_RETURN`, `RETURN_ON_ERROR` und `CHECK`. Die Fehlermeldung (Datei, Zeile, Text) erscheint auf dem LCD.
- Fehlercodes werden bis `main` durchgereicht. Die Meldung bleibt stehen, bis S5 das nächste Bild anfordert. Restbytes der fehlerhaften Datei verwirft `open_nxt_file()`.

| Fehlerfall | Code |
|---|---|
| Header ungültig (Signatur, biSize, biPlanes, biBitCount, Kompression …) | `HEADER_ERR` (headers.c) |
| Bild größer als 2400 × 1600 (5 × Display) | `IMG_SIZE_ERR` |
| Farbtabelle unvollständig, ragt in die Pixeldaten oder Index ≥ Anzahl Farben | `COL_TAB_ERR` |
| RLE: Zeile länger als biWidth, mehr Zeilen als biHeight, Delta hinter den Rand | `FORMAT_ERR` |
| Datei endet vorzeitig (kein „End of bitmap“) | `READ_ERR` |
| andere Kodierung | `UNSUPPORTED_FORMAT_ERR` |

## 3. Box-Algorithmus (Teilaufgabe c)

- Skalierungsfaktor für beide Achsen gleich (keine Verzerrung): Das Bild wird so verkleinert, dass es ins Display passt. Beispiel: 1440 × 1280 → 360 × 320.
- Abbildung je Achse: `Displayposition = floor(Bildposition * n_out / n_in)`. Alle Bildpixel mit derselben Displayposition bilden eine Kachel. Bei Faktor 1/5 ist eine Kachel 5 × 5 Pixel groß.
- Statt der letzten 5 Bildzeilen hält `lineHandling` pro Display-Spalte nur die **Summen** von Rot, Grün und Blau (3 × 480 × 4 Byte). Jede Bildzeile wird beim Dekodieren sofort aufaddiert. Wechselt die Bildzeile in die nächste Kachelzeile, werden die Mittelwerte gebildet (Summe / Pixel der Kachel, gerundet) und als **eine** LCD-Zeile mit `GUI_WriteLine` ausgegeben.
- Speicher: ca. 6,7 KB statt 5 × 2400 × 3 Byte = 36 KB für 5 gepufferte Zeilen.
- Bilder ≤ 480 × 320 werden nicht verkleinert (Kachel = 1 Pixel), sondern 1:1 links oben dargestellt.

## 4. Schalter (bmp_config.h)

| Teilaufgabe | `USE_LINE_OUTPUT` | `USE_SCALING` | Verhalten |
|---|---|---|---|
| a | 0 | 0 | jedes Pixel mit `GUI_drawPoint`, große Bilder abgeschnitten |
| b | 1 | 0 | Zeile mit `GUI_WriteLine`, große Bilder abgeschnitten |
| c (Abgabe) | 1 | 1 | Zeile mit `GUI_WriteLine`, große Bilder verkleinert |

`SHOW_HEADERS = 1` zeigt vor jedem Bild beide Header an (`input_file_header()`, weiter mit S5). `SHOW_DRAW_TIME = 1` zeigt die Zeichendauer in ms an (Vergleich a ↔ b).

## 5. Tests

Host-Test: Der komplette Dekoder (alle eigenen Module + `headers.c` + `errorhandler.c`) wurde auf dem PC mit nachgebildetem LCD und Eingabe übersetzt (`Test/`, mit AddressSanitizer) und gegen eine Referenz (Pillow) verglichen.

| Testbild | Erwartung | Ergebnis (Host) |
|---|---|---|
| 22x14, 60x50, 6x10 (unkomprimiert, Padding) | 1:1 | pixelgenau ✓ |
| 480x320 ITSboard, 479x320 RECBAR, 480x270 SecVI, red_frame, some_colors | 1:1 | pixelgenau ✓ |
| 480x320 red_frame uncompressed (8 Bit BI_RGB) | 1:1 | pixelgenau ✓ |
| 580x400, 640x480, 720x400, 1280x854, 1440x1280, 2400x1600 | verkleinert | wie Referenz-Box-Filter ✓ |
| 640x480 24 Bit unkomprimiert | verkleinert | ✓ |
| 128x480 „zeile_zu_lang“ | Fehler | `FORMAT_ERR` ✓ |
| 640x480 „biWidth zu groß / viel zu groß“ | Fehler | `IMG_SIZE_ERR` ✓ |
| 4488x2525, 6000x4000 | > 5 × Display | `IMG_SIZE_ERR` ✓ |
| BildSignaturDefekt | Fehler | `HEADER_ERR` ✓ |
| RLE-Beispiel aus der Aufgabenstellung (mit Delta, Absolute Mode) | wie Tabelle | ✓ |
| 300 zufällig beschädigte / abgeschnittene Dateien | Fehlermeldung, kein Absturz | ✓ |

Tests auf dem ITS-Board (im Labor ausfüllen):

| Test | Erwartung | Ergebnis |
|---|---|---|
| alle Bilder aus `BITMAP_list.txt` nacheinander mit S5 | wie oben | |
| Teilaufgabe a gegen b (`SHOW_DRAW_TIME`) | b deutlich schneller | a: ___ ms, b: ___ ms |
| Fehlerbild, danach gültiges Bild | Meldung, dann Bild korrekt | |

## 6. Projekt-Einstellungen (Aufgabe3.cproject.yml)

Zusätzlich zu den Einträgen aus `ANLEITUNG.pdf` (Include-Pfade, `HAL_PCD_MODULE_ENABLED`, Gruppe `Middleware/USB`) in der Gruppe `Program/User/Src`:

```yaml
        - file: Src/bmpApp.c
        - file: Src/colorTable.c
        - file: Src/compressedBMP.c
        - file: Src/uncompressedBMP.c
        - file: Src/lineHandling.c
        - file: Src/lcdOutput.c
```

Stack: Alle großen Puffer sind `static`. Falls trotzdem ein Stack-Overflow auftritt, die Stack-Größe auf 0x800 erhöhen (siehe `README_stackoverflow.md`).
