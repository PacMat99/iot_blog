---
title: Setup NodeRED
layout: home
---

# Setup di NodeRED su Raspberry

## 1 - Installazione del client Mosquitto

Esegui da terminale i seguenti comandi:

    sudo apt update && sudo apt upgrade
    sudo apt install -y mosquitto mosquitto-clients
    sudo systemctl enable mosquitto.service
    mosquitto -v

## 2 - Abilitazione dell'accesso remoto e autenticazione

Apri il file mosquitto.conf con l'editor che preferisci:

    sudo vim /etc/mosquitto/mosquitto.conf

e incolla quanto segue:

    listener 1883
    allow_anonymous true

Esegui il comando seguente sostituendo YOUR_USERNAME con il nome dell'utente che utilizzerai per loggarti e memorizza nome utente e password:

    sudo mosquitto_passwd -c /etc/mosquitto/passwd YOUR_USERNAME

Apri il file mosquitto.conf con l'editor che preferisci:

    sudo vim /etc/mosquitto/mosquitto.conf

e aggiungi la riga seguente all'inizio del file per permettere solo connessioni autenticate:

    per_listener_settings true

e le righe seguenti alla fine del file:

    allow_anonymous false
    listener 1883
    password_file /etc/mosquitto/passwd

Infine riavvia Mosquitto:

    sudo systemctl restart mosquitto