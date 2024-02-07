---
title: Setup NodeRED
layout: default
nav_order: 3
---

# Setup di NodeRED su Raspberry

## 1 - Installazione del client Mosquitto

Eseguire da terminale i seguenti comandi:

```
sudo apt update && sudo apt upgrade
sudo apt install -y mosquitto mosquitto-clients
sudo systemctl enable mosquitto.service
mosquitto -v
```

## 2 - Abilitazione dell'accesso remoto e autenticazione

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
