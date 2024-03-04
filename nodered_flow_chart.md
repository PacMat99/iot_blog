---
title: Node-RED flow chart
layout: default
nav_order: 8
---


TODO: AGGIUNGERE CASO CON INVIO DI SEGNALE TRAMITE NODERED AD ESEMPIO PER ACCENDERE UN RELÈ



# Creazione del flow chart in Node-RED e della UI

## 1. Fare qualche test coi presenti in Node-RED

Iniziamo creando un flow chart coi nodi riportati di seguito e configuriamoli come indicato:

![Test Flow](./images/nodered/test_flow.png)

Configurare l'inject node:

![Test Inject Node](./images/nodered/test_inject_node.png)

Configurare il debug node:

![Test Debug Node](./images/nodered/test_debug_node.png)

Se tutto è stato configurato correttamente, cliccando su *Deploy* e successivamente sul quadratino blu dell'inject node dovremmo vedere che riceviamo le stringe inviate al debug:

![Test Debug Node](./images/nodered/test_debug_output.png)

Per vedere la UI di Node-RED è necessario collegarsi alla pagina `raspberryip:1880/ui/`

## 2. Creazione del flow chart finale e della UI

Ora che abbiamo preso un po' di dimistichezza con l'utilizzo di Node-RED possiamo creare il flow chart finale che ci permetterà di far funzionare il nostro *sistema IoT* nella sua interezza:

![Final Flow](./images/nodered/final_flow.png)

Configuriamo il nodo *mqtt_in*

![MQTT Node Config](./images/nodered/mqtt_node_config.png)

la sua connessione col *broker MQTT*

![MQTT Broker Config](./images/nodered/mqtt_broker_config.png)

e le *credenziali di accesso*

![MQTT Credentials Config](./images/nodered/mqtt_credentials_config.png)

Configuriamo il nodo funzione che ci permetterà di creare la query con cui inserire nel DB i dati ricevuti dal microcontrollore

![Query Function](./images/nodered/query_function.png)

e il nodo per la connessione al DB

![MySQL Node](./images/nodered/mysql_node.png)

Se abbiamo configurato tutti i nodi correttamente e clicchiamo su **Deploy**, andando nel tab di *Debug* di Node-RED dovremmo vedere le query che vengono inviate al nostro DB

![Debug Output](./images/nodered/debug_output.png)

allo stesso tempo dovremmo veder variare i grafici nella UI

![Node-RED UI](./images/nodered/nodered_ui.png)

e il database dovrebbe popolarsi coi dati raccolti dal sensore

![DB Table](./images/db/db_table.png)

Avanti al [prossimo step](./index.html)! :)
