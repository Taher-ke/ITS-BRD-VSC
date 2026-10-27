## Fehlerrückmeldung

1. Kritische Funktionen geben einen Integer-Wert zurück. Dieser gibt den Status nach der Ausführung der Funktion wieder. 
2. Der Status-Wert wird nach jedem Aufruf der jeweiligen Funktion geprüft. Ist er negativ, liegt ein Fehler vor. Dann wird der Status-Wert bis in die Main-Methode hochgereicht und dort von dem Output-Modul bearbeitet. 
3. Den unterschiedlichen negativen Status-Werten wird eine eindeutige Fehlermeldung zugeordnet und diese auf dem Terminal ausgegeben.
4. Anschließend wird darauf gewartet, dass der Benutzer den Fehler quittiert und darauf der Taschenrechner neu gestartet.

Sollen die Funktionen Ergebnisse zurückgeben, nutzen sie kein "return". Stattdessen wird ihnen als Parameter eine Referenz auf eine Variable übergeben, in die sie den Rückgabewert schreiben.  