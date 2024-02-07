---
title: Setup ESP
layout: home
---

# ESP8266 / ESP32 configuration

## 1 - Install Arduino IDE

Download and install Arduino IDE from [this site](https://www.arduino.cc/en/software)

## 2 - Install esptool and flash ESP32 fw

	esptool.py --port USBPORT erase_flash
	esptool.py --port USBPORT --baud 460800 write_flash --flash_size=detect 0x1000 esp32_file_name.bin

## 3 - Install ESP32 board on Arduino IDE

1. Add https://espressif.github.io/arduino-esp32/package_esp32_index.json to "Additional Boards Manager URLs" in Arduino IDE settings.
2. Install ESP32 board by Espressif from board manager

## 3.1 (if required) - Install CH340 driver on Ubuntu

	sudo apt remove brltty
	sudo apt update
	sudo apt install neofetch -y
	neofetch

-------- if kernel version is before 6.x --------

	sudo add-apt-repository ppa:cappelikan/ppa
	sudo apt update -y
	sudo apt install mainline -y
	sudo apt autoremove -y

-------- end if --------

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
