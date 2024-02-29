---
title: ESP code
layout: default
nav_order: 3
has_children: true
---



AGGIUNGERE INSTALLAZIONE LIBRERIA PUBSUBCLIENT

AGGIUNGERE CASO CON INVIO DI SEGNALE TRAMITE NODERED AD ESEMPIO PER ACCENDERE UN RELÈ


# Test ESP8266 / ESP32

Per testare il corretto funzionamento dell'ESP è consigliabile procedere per step in modo da poter isolare al meglio i singoli problemi che possiamo riscontrare.  
Ad esempio potremmo procedere in quest'ordine:

1. Far lampeggiare il led integrato dell'ESP
2. Leggere e scrivere su terminale i valori letti dal sensore SDS011
3. Provare a scrivere delle semplici frasi sul display SSD1306
4. Scrivere sul display SSD1306 i valori letti dal sensore SDS011 in modo da permetterne una visualizzazione ottimale
5. Testare il funzionamento del WiFi dell'ESP
6. Testare l'invio di dati tramite MQTT

Avanti al [prossimo step](../mosquitto_setup.html)! :)
