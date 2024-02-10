---
title: Setup Node-RED
layout: default
nav_order: 4
---

# Setup di Node-RED su Raspberry Pi

## 0. Cos'è Node-RED

Node-RED is a powerful open-source tool for building Internet of Things (IoT) applications with the goal of simplifying the programming component.

Node-RED runs on the web browser and it uses visual programming that allows you to connect code blocks, known as nodes, together to perform a task. The nodes when wired together are called flows.

Why is Node-RED a great solution?
Node-RED is open source and developed by IBM.
The Raspberry Pi runs Node-RED perfectly.
It is a visual programming tool, which makes it more accessible to a wider range of users.
With Node-RED you can spend more time making cool stuff, rather than spending countless hours writing code.
What can you do with Node-RED?
Node-RED makes it easy to:

Access your RPi GPIOs;
Establish an MQTT connection with other devices (Arduino, ESP8266, ESP32 etc);
Create a responsive graphical user interface for your projects;
Communicate with third-party services (IFTTT.com, Adafruit.io, ThingSpeak, Home Assistant, InfluxDB etc);
Retrieve data from the web (weather forecast, stock prices, emails. etc);
Create time-triggered events;
Store and retrieve data from a database.

![Node-RED](./images/home_automation_example.png)  
*Node-RED*

Puoi approfondire ulteriormente [qui](https://mosquitto.org/) e trovare degli esempi di utilizzo [qui](https://flows.nodered.org/).

## 1. Installazione di Node-RED su Raspberry Pi

Lanciare il seguente comando da terminale:

```
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
```

e a installazione finita vi troverete di fronte ad una schermata simile a quella seguente:

![Node-RED Install](./images/nodered_install.png)

Successivamente sarà necessario configurare Node-RED come impostando nome utente e passphrase e confermando le impostazioni di default delle altre voci:

![Node-RED Config](./images/nodered_config.png)

Ora possiamo avviare Node-RED per verificare che l'installazione e la configurazione siano state eseguite correttamente. Lanciando da terminale il comando

```
node-red-start
```

dovremmo trovarci di fronte ad una schermata simile a questa:

![Node-RED Start](./images/nodered_start.png)

Infine abilitiamo il servizio di Node-RED in modo che si avvii in automatico all'accensione del Raspberry Pi:

```
sudo systemctl enable nodered
```

Avanti al [prossimo step](./index.html)! :)
