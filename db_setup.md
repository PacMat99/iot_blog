---
title: Setup DB
layout: default
nav_order: 7
---

# Setup di un LAMP server e creazione di un DB su Raspberry Pi

## 1. Configurare un LAMP (Linux, Apache, MySQL, PHP) Server

### Aggiornare il sistema

```
sudo apt update && upgrade -y
```

### Installare Apache e PHP

```
sudo apt install apache2 php
```

### Installare MariaDB e il supporto a MySQL per PHP

```
sudo apt install mariadb-server php-mysql
```

### Installare phpMyAdmin.  

```
sudo apt install phpmyadmin
```

Durante l'installazione viene richiesto qualce web server deve essere configurato per l'esecuzione di phpMyAdmin: selezionare apache2 con la barra spaziatrice e premere invio per confermare.

A fine installazione confermare la configurazione automatica del DB con dbconfig-common.

Infine, abilitare l'avvio automatico di MariaDB all'avvio, l'estensione PHP MySQLi e riavviare Apache2 coi seguenti comandi:

```
sudo systemctl enable mariadb
sudo phpenmod mysqli
sudo service apache2 restart
```

## 2. Creare un utente e un DB MySQL

Creating a MySQL Database and User
1. Before we proceed to create a MySQL user and database on our Raspberry Pi, we must first log back into the MySQL command-line tool.

Run the following command to log in to the MySQL command line. You will be prompted to enter the password for the “root” account that you set up earlier.

sudo mysql -u root -p
Copy
2. Let’s start by creating a MySQL database using the following command.

This command is super simple and is just “CREATE DATABASE” followed by the name that you want to give the database.

In our example, we will be calling this database “exampledb“.

CREATE DATABASE exampledb;
Copy
3. Next, we will create a MySQL user that we will assign to our new database. We can create this user by running the following command.

For this example, we will be calling the user “exampleuser” and giving it the password “pimylifeup“. When creating your own, make sure you replace both of these.

CREATE USER 'exampleuser'@'localhost' IDENTIFIED BY 'pimylifeup';
Copy
4. With the user created, we can now go ahead and grant all privileges to the user so that it can interact with the database.

This command will grant all permissions to our “exampleuser” for all tables within our “exampledb” database.

GRANT ALL PRIVILEGES ON exampledb.* TO 'exampleuser'@'localhost';
Copy
5. The final thing we need to do for both our MySQL database and user to be finalized is to flush the privilege table. Without flushing the privilege table, the new user won’t be able to access the database.

We can do this by running the following command.

FLUSH PRIVILEGES;
