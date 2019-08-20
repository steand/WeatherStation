# Wetterstation

Version 0.1 *(under construction, not stabil)*

In diesem Projekt habe ich mir eine kleine Wetterstation gebaut.
Meine Motivation war, dass es eigentlich nichts erschwingliches gibt was in die offenen Smart-Home (bei mir openHAB2) integrierbar ist.

Leistungsmerkmale:
- Windgeschwindigkeit
- Windrichtung
- Regenmenge
- Regensensor
- Temperatur
- Luftdruck
- Luftfeuchtigkeit
- Helligkeit

Zur Steuerung meine Markise benötige ich Alarme für Regen und Windgeschwindigkeit.
Die Rolladen werden mit Hilfe der Helligkeitssensor gesteuert.

Für Windgeschwindigkeit, -richtung und Regenmenge dienen die Sensoren eine AOK5055.
Regensensor ist ein "China Modul".
Temperatur, Luftdruck und -feuchte basieren auf einem BME280,
Helligkeit auf dem BH1750.

Die Microcontroller Basis ist  ESP-WROOM-32u (u wegen der Antenne).

Als Stromversorung dient eine 18500/3000mAh Batterie die über ein Modul(TP4056) geladen wird.
Der Lader wird über ein MPPT-Modul (China) von einem 5W/12V Solarpenel versorgt.
Als SDK setze ich platformIO auf Basis von Atom ein.
Für Schaltpläne und Platinenlayout (Entwicklerplatine) nutze ich KiCad.

Weitere Details unter dem Verzeichnis ```
./doc
```.



