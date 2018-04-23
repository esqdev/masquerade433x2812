# masquerade433x2812
Sourcecode für die Gruppenkostüme 2019


## Master
ESP8266 der als Webserver und als Signalgeber fungiert

## Slave
Arduino oder ATMegaXXX der die 433Mhz Signale empfängt, das ensprechende Programm aufruft und an die WS2812 sendet.

## WebApp
HTML Webseite die Buttons ausspielt
- als include für das masterDevice.ino
- als .html zum offline-testen
