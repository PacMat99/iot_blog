---
title: Setup DB
layout: default
nav_order: 7
---

# Setup di Node-RED su Raspberry Pi

## 0. Cos'è Node-RED

## 1. Installazione di Node-RED su Raspberry Pi

sudo apt update
sudo apt upgrade
sudo apt install mariadb-server


With the MySQL server software installed on the Raspberry Pi, we will now need to secure it by setting a password for the “root” user.

By default, MySQL is installed without any password configured, meaning you can access the MySQL server without any authentication.

Run the following command to begin the MySQL securing process.

sudo mysql_secure_installation
Copy
Just follow the prompts to set a password for the root user and to secure your MySQL installation.

For a more secure installation, you should answer “Y” to all prompts when asked to answer “Y” or “N“.

These prompts will remove features that allows someone to gain access to the server easier.

Make sure you write down the password you set during this process as we will need to use it to access the MySQL server and create databases and users for software such as WordPress or PHPMyAdmin.

4. Now, if you want to access your Raspberry Pi’s MySQL server and start making changes to your databases, you can enter the following command.

sudo mysql -u root -p
Copy
5. You will be prompted to enter the password that we just created in step 3 for MySQL’s root user.

Note: Like most Linux password inputs, the text will not appear as you type.

6. You can now enter MYSQL commands to create, alter, and delete databases. Through this interface, you can also create or delete users and assign them the rights to manage any database.

7. There are two different ways you can quit out of the MYSQL command line. The first of those is to type “quit;” into the MySQL interface.

The other way of quitting out of the MYSQL command line is to press CTRL + D.

8. At this point, you will now have successfully set up MySQL on your Raspberry Pi. Our next few sections will go into making better use of this database.



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




enable mariadb at startup



sudo systemctl enable mariadb