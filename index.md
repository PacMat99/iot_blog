---
title: Home
layout: home
nav_order: 1
---

# Sistema IoT con Node-RED

In questa repo troverai del materiale utile a capire come collegare un ESP con un sensore sds011, un display e un modulo LoRa, collezionare i dati raccolti dal sensore e inviarli al Raspberry.

## 1. Assicurarsi di avere tutti i componenti necessari:

1 x Raspberry Pi  
1 x scheda SD per Raspberry Pi  
1 x Alimentatore per Raspberry Pi  
1 x ESP8266 o ESP32  
1 x display SSD1306  
1 x SDS011  
1 x modulo LoRa  
1 x breadboard  
Y x cavetti per collegare l'ESP a periferiche esterne  

## 2. Configurare l'Arduino IDE e il pc

Configurare l'Arduino IDE e il pc affinché siano in grado di riconoscere e programmare l'ESP8266 / ESP32 come indicato in ["Setup ESP"](./esp_setup.html).

## 3. Testare il corretto funzionamento dell'ESPx e di tutti i componenti collegati

Per testare il corretto funzionamento dell'ESPx è consigliabile procedere per step in modo da poter isolare al meglio i singoli problemi che possiamo riscontrare.

Se vuoi saperne di più su come eseguire questi test, trovi diverse info [qua](./esp_tests.html)

## 4. Creare lo schema elettrico

Creare lo schema elettrico mostrato nel [test](./tests/sds011_display_test.html) di ESPx, display SDS011 e display SSD1306.

## 5. Installare Mosquitto su Raspberry Pi

Installare Mosquitto sul Raspberry Pi come indicato in ["Setup Mosquitto"](./mosquitto_setup.html).

## 6. Installare Node-RED su Raspberry Pi

Installare Node-RED sul Raspberry Pi come indicato in ["Setup Node-RED"](./nodered_setup.html).

----

Made with &hearts; by [PacMat99](https://github.com/PacMat99)