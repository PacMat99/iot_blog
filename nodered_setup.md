---
title: Setup NodeRED
layout: default
nav_order: 3
---

# Setup di NodeRED su Raspberry Pi

## 1. Installazione del client Mosquitto

Eseguire da terminale i seguenti comandi:

```
sudo apt update && sudo apt upgrade
sudo apt install -y mosquitto mosquitto-clients
sudo systemctl enable mosquitto.service
mosquitto -v
```

## 2. Abilitazione dell'accesso remoto e autenticazione

Aprire il file mosquitto.conf con l'editor preferito:

```
sudo vim /etc/mosquitto/mosquitto.conf
```

e incollare quanto segue:

```
listener 1883
allow_anonymous true
```

Eseguire il comando seguente sostituendo YOUR_USERNAME con il nome dell'utente che utilizzato per loggarsi e memorizzare nome utente e password:

```
sudo mosquitto_passwd -c /etc/mosquitto/passwd YOUR_USERNAME
```

Aprire il file mosquitto.conf con l'editor preferito:

```
sudo vim /etc/mosquitto/mosquitto.conf
```

e aggiungere la riga seguente all'inizio del file per permettere solo connessioni autenticate:

```
per_listener_settings true
```

e le righe seguenti alla fine del file:

```
allow_anonymous false
listener 1883
password_file /etc/mosquitto/passwd
```

Infine riavviare Mosquitto:

```
sudo systemctl restart mosquitto
```
## 3. Installazione di Node-RED su Raspberry Pi

Lanciare il seguente comando da terminale:

```
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
```

e a installazione finita vi troverete di fronte ad una schermata simile a quella seguente:

![NodeRED Install](./images/nodered_install.png)

Successivamente sarà necessario configurare NodeRED come impostando nome utente e passphrase e confermando le impostazioni di default delle altre voci:

![NodeRED Config](./images/nodered_config.png)

Ora possiamo avviare NodeRED per verificare che l'installazione e la configurazione siano state eseguite correttamente. Lanciando da terminale il comando

```
node-red-start
```

dovremmo trovarci di fronte ad una schermata simile a questa:

![NodeRED Start](./images/nodered_start.png)

Infine abilitiamo il servizio di NodeRED in modo che si avvii in automatico all'accensione del Raspberry Pi:

```
sudo systemctl enable nodered
```

Avanti al [prossimo step](./index.html)! :)
