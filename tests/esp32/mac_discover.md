---
title: MAC discover
layout: default
nav_order: 7
parent: ESP32
grand_parent: ESP code
---

# Scoprire il MAC address dell'ESP32

Collegare l'ESP32 al pc, copiare il codice seguente in un file nell'Arduino IDE e caricarlo sul microcontrollore.  
Il codice stampa ogni 10 secondi sul display seriale il MAC address dell'ESP32.

```
#include <WiFi.h>

void setup(){
  Serial.begin(115200);
  Serial.println();
}
 
void loop(){
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  delay(10000)
}
```
