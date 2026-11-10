# GS Praktikumsaufgabe 2 – Drehgeber mit Polling

Team: Taher Keshavarzian, Sönke Wulf

## 1. Modulkonzept

| Modul | Aufgabe | Hardware | Schnittstelle |
|---|---|---|---|
| `main.c` | Super-Loop nach DDC: einlesen, berechnen, ausgeben; Reset und zentrale Fehlerbehandlung | – | – |
| `rotary_input` | Phase (PF0 = A, PF1 = B) mit Zeitstempel und Taste S6 (PF6) einlesen | GPIOF->IDR, Timer | `read_phase()`, `button_s6_pressed()` |
| `rotary_calculator` | FSM (Phasenwechsel), Phasenzähler mit Zeitstempel, Winkel, Winkelgeschwindigkeit | keine | `rotary_reset()`, `rotary_update()`, `rotary_get_counter()`, `rotary_get_angle()`, `rotary_get_speed()` |
| `rotary_led` | D8–D15 Zähler, D21 Fehler, D22/D23 Richtung | GPIOD/GPIOE->BSRR | `set:led_counter()`, `set_led_direction()`, `clear_direction_leds()`, `set_error_led()`, `clear_error_led()` |
| `rotary_display` | Feste Texte einmalig, Werte zeichenweise (max. 1 Zeichen pro Durchlauf) | LCD | `init_display()`, `update_display()` |
| `rotary_states.h` | Gemeinsame Konstanten: Phasen, STATE_*, Fehlercodes, `Phase_t` | – | – |

`rotary_calculator` greift nicht auf Hardware zu und wird deshalb mit `Test/test_rotary_calculator.c` auf dem PC getestet.

### Kodierung der Phasen (Wert = (B << 1) | A)

| Phase | A (PF0) | B (PF1) | Rohwert |
|---|---|---|---|
| a | 0 | 0 | 0 |
| b | 1 | 0 | 1 |
| c | 1 | 1 | 3 |
| d | 0 | 1 | 2 |

## 2. Eingabezustände und Reaktion

| Eingabe | Bedingung | Reaktion |
|---|---|---|
| Keine Änderung | Phase = letzte Phase | Zähler unverändert, Richtungs-LED unverändert |
| Vorwärts | a→b, b→c, c→d, d→a | Zähler +1, D23 an, D22 aus |
| Rückwärts | b→a, c→b, d→c, a→d | Zähler −1, D22 an, D23 aus |
| Phase übersprungen | a↔c, b↔d | D21 an; Winkel, Geschwindigkeit und LEDs eingefroren |
| S6 gedrückt | jederzeit | Reset: Zähler 0, Winkel/Geschwindigkeit 0, D21/D22/D23 aus, Fehler gelöscht |
| Kein Phasenwechsel ≥ 3 s | Stillstand | Geschwindigkeit wird neu berechnet (0,0 °/s) |

## 3. Konzept der Fehlerbehandlung

- Fehlercodes in `rotary_states.h`: `EOK = 0`, Fehler negativ (`ERR_PHASE_SKIPPED`, `ERR_INVALID_PARAM`).
- Module schalten keine Fehler-LED selbst, sondern melden den Fehler an `main.c`.
- `handleError()` in `main.c` ist die einzige Stelle, die D21 einschaltet und den Fehlerzustand setzt.
- Im Fehlerzustand werden keine Werte mehr berechnet. Das Display beendet nur den angefangenen Ausgabezyklus mit den eingefrorenen Werten (keine halb geschriebenen Zahlen).
- Nur S6 verlässt den Fehlerzustand (`resetProgram()`).

## 4. Winkelgeschwindigkeit

- 1200 Phasenwechsel pro Umdrehung → 0,3° pro Phasenwechsel.
- Der Phasenzähler wird zusammen mit dem Zeitstempel des letzten Phasenwechsels gespeichert (`Counter_t`).
- Ein Zeitfenster endet, wenn ≥ 300 ms vergangen sind und im aktuellen Durchlauf ein Phasenwechsel erkannt wurde. Anfang und Ende liegen damit direkt hinter Phasenwechseln.
- Ohne Phasenwechsel wird spätestens nach 3 s neu berechnet (bei 0,1 °/s liegen 3 s zwischen zwei Phasenwechseln).
- Zeitdifferenzen werden als `uint32_t` gebildet: korrekt auch über einen Timer-Überlauf, da ein Fenster höchstens 3 s ≪ 47,7 s dauert.

## 5. Tests

### Host-Test (ohne Board)

```
gcc -std=c11 -Wall -IInc Test/test_rotary_calculator.c Src/rotary_calculator.c -lm -o test && ./test
```

| Test | Erwartung | Ergebnis |
|---|---|---|
| T1 alle 16 Phasenübergänge, ungültiger Parameter | laut Tabelle 1 | OK |
| T2 1200 Schritte vorwärts, 1 Schritt rückwärts | 360,0°, Zähler −1 | OK |
| T3 90 °/s, auch über Timer-Überlauf | 90,0 °/s | OK |
| T4 Stillstand | Ausgabe erst nach 3 s, 0,0 °/s | OK |
| T5 0,1 °/s | 0,1 °/s statt 0 | OK |

### Tests auf dem ITS-Board (im Labor ausfüllen)

| Test | Werkzeug | Erwartung | Ergebnis |
|---|---|---|---|
| Langsam einen Schritt vorwärts | Drehgeber | Zähler 1, 0,3°, D23 an | |
| Ein Schritt rückwärts ab 0 | Drehgeber | LEDs 0xFF, −0,3°, D22 an | |
| Genau eine Umdrehung | Pi Pico | 360,0° | |
| 90 °/s konstant | Pi Pico | 90,0 ± 0,1 °/s | |
| 0,1 °/s | Pi Pico | 0,1 °/s | |
| Rückwärts mit konstanter Geschwindigkeit | Pi Pico | negativer Wert, D22 an | |
| Zu schnell drehen | Pi Pico / Drehgeber | D21 an, Anzeige eingefroren | |
| S6 im Fehlerzustand und im Normalbetrieb | Taste | alles auf 0, Fehler gelöscht | |
| > 47 s Stillstand, dann drehen | Drehgeber | Geschwindigkeit korrekt | |
| Mehr als 255 Schritte | Pi Pico | LEDs zeigen untere 8 Bit | |
| Höchste fehlerfreie Geschwindigkeit | Pi Pico | ≈ 0,3° / T_Loop,max | |

## 6. Analyse des Zeitverhaltens (Oszilloskop)

In `main.c` `MEASURE_TIMING` auf 1 setzen. PE0 ist dann während `updateDisplay()` HIGH. Für die Dauer eines ganzen Durchlaufs den Pin am Anfang der Schleife setzen und am Ende zurücksetzen. Masse des Oszilloskops anschließen.

| Messung | Ergebnis |
|---|---|
| Ausgabe eines Zeichens auf dem LCD | ___ µs |
| Ausgabe beider Werte komplett (18 Zeichen) | ___ µs |
| Super-Loop-Durchlauf ohne LCD-Ausgabe | ___ µs |
| Längster Super-Loop-Durchlauf (zeichenweise Ausgabe) | ___ µs |

Maximale verarbeitbare Winkelgeschwindigkeit: ω_max = 0,3° / T_Loop,max = ___ °/s.

## 7. Änderungen gegenüber der ersten Version

1. DDC: Beim Reset wird kein zweites Mal eingelesen, sondern die Phase des aktuellen Durchlaufs verwendet.
2. Reset vollständig (Zähler, Winkel, Geschwindigkeit, Zeitfenster, Richtungs-LEDs) und jederzeit mit S6 möglich.
3. Geschwindigkeit nach Aufgabenstellung: 300-ms-Fenster, Timeout 3 s, Zeitstempel am Phasenwechsel.
4. Phasenzähler, Winkel und Geschwindigkeit ins Modul `rotary_calculator` verschoben (Zustand in `static`-Variablen). Die zuvor nur deklarierten Funktionen `rotary_get_angle()` und `rotary_get_speed()` sind jetzt implementiert.
5. `Phase_t` (Phase + Zeitstempel) und `Counter_t` (Zähler + Zeitstempel) als struct.
6. Phasenkonstanten korrigiert (vorher Hex-Werte 0x10/0x11 und vertauschte Kommentare). Die FSM-Matrix nutzt Designated Initializers mit diesen Konstanten, mit Bereichsprüfung der Parameter.
7. Im Fehlerfall wird sofort abgebrochen. Das Display beendet den laufenden Zyklus mit eingefrorenen Werten.
8. Display: genau ein Zeichen pro Durchlauf, Puffer `static`.
9. Einheitliche Fehlerbehandlung (`handle_error()`, Fehlercodes).
10. Aufräumen: ungenutzte Makros und Deklarationen entfernt, `main.h` (Inhalt „empty file“) entfernt, Konstanten in `rotary_states.h`, `1U <<` statt `1 <<`, `default` in `switch`, mit clang-format formatiert.
