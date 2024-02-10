---
title: ESPx tests
layout: default
nav_order: 2
has_children: true
---

# Test ESP8266 / ESP32

Per testare il corretto funzionamento dell'ESPx è consigliabile procedere per step in modo da poter isolare al meglio i singoli problemi che possiamo riscontrare.  
Ad esempio potremmo procedere in quest'ordine:

1. Far lampeggiare il led integrato dell'ESPx
2. Leggere e scrivere su terminale i valori letti dal sensore SDS011
3. Provare a scrivere delle semplici frasi sul display SSD1306
4. Scrivere sul display SSD130 i valori letti dal sensore SDS011 in modo da permetterne una visualizzazione ottimale
5. Testare il funzionamento del WiFi dell'ESPx
