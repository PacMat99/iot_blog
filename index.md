---
title: Home
layout: home
nav_order: 1
---

# Sistema IoT con Node-RED

In questa repo troverai del materiale utile a capire come collegare un ESP con un sensore sds011, un display, un led e un modulo LoRa, collezionare i dati raccolti dal sensore e inviarli al Raspberry.

![Node-Red IoT System](../../images/wiring_diagrams/iot_system.png)

## 1. Configurare l'Arduino IDE e il pc

Configurare l'Arduino IDE e il pc affinché siano in grado di riconoscere e programmare l'ESP8266 / ESP32 come indicato in ["Setup ESP"](./esp_setup.html).

## 2. Testare il corretto funzionamento dell'ESP e di tutti i componenti collegati

Per testare il corretto funzionamento dell'ESP è consigliabile procedere per step in modo da poter isolare al meglio i singoli problemi che possiamo riscontrare.

Se vuoi saperne di più su come eseguire questi test, trovi diverse info [qui](./tests/esp_tests.html)

## 3. Creare lo schema elettrico

Creare lo schema elettrico mostrato nei [test](./tests/esp_tests.html) di ESP, display SDS011 e display SSD1306.

## 4. Installare Mosquitto su Raspberry Pi

Installare Mosquitto sul Raspberry Pi come indicato in ["Setup Mosquitto"](./mosquitto_setup.html).

## 5. Installare Node-RED su Raspberry Pi e configurare la Dashboard

Installare Node-RED sul Raspberry Pi come indicato in ["Setup Node-RED"](./nodered_setup.html).

## 6. Configurare la Dashboard di Node-RED su Raspberry Pi

Configurare la Dashboard di Node-RED su Raspberry Picome indicato in ["Configurazione Node-RED"](./nodered_config.html).

## 7. Installare e configurare un DB MySQL

Installare e configurare un DB MySQL come indicato in ["Setup DB"](./db_setup.html).

## 8. Creare il Flow Chart in Node-RED

Prendere dimistichezza con l'interfaccia di Node-RED e creare il Flow Chart complessivo come indicato in ["Node-RED Flow Chart"](./nodered_flow_chart.html).

## 9. Creare un sistema IoT completo

Prendendo spunto dagli step precedenti, creare un sistema IoT che permetta lo scambio di dati tra l'ESP, il Raspberry Pi e la rete LoRa per raccogliere i dati dai sensori, salvarli nel DB MySQL e condividerli sulla rete LoRa come indicato in ["IoT System"](./iot_system.html).

----

Made with &hearts; by [PacMat99](https://github.com/PacMat99)