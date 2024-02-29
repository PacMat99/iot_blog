---
title: MAC discover
layout: default
nav_order: 7
parent: ESP8266
grand_parent: ESP code
---

# Scoprire il MAC address dell'ESP8266

Collegare l'ESP8266 al pc, copiare il codice seguente in un file nell'Arduino IDE e caricarlo sul microcontrollore.  
Il codice stampa ogni 10 secondi sul display seriale il MAC address dell'ESP8266.

```
#include <ESP8266WiFi.h>

void setup(){
  Serial.begin(115200);
  Serial.println();
}
 
void loop(){
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  delay(10000)
}
```
