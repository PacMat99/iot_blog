---
title: CH340 driver
layout: default
nav_order: 1
parent: Help
---

# Installare i driver su Ubuntu per il chip CH340

```
sudo apt remove brltty
sudo apt update
sudo apt install neofetch -y
neofetch
```

----
if kernel version is before 6.x

```
sudo add-apt-repository ppa:cappelikan/ppa
sudo apt update -y
sudo apt install mainline -y
sudo apt autoremove -y
```
----

```
sudo apt-get install build-essential dwarves dkms make cmake -y
sudo apt autoremove -y
git clone https://github.com/juliagoda/CH341SER
cd CH341SER
sudo make clean 
cp /sys/kernel/btf/vmlinux /usr/lib/modules/`uname -r`/build/
sudo make
sudo make load
lsmod | grep ch34*
sudo usermod -a -G dialout $(whoami)
sudo chmod a+rw /dev/ttyUSB0
```
