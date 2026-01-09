# Raspberry Pi UART Setup Guide

## 1. Enable UART (GPIO14 & GPIO15 - Pins 8 & 10)

Edit boot configuration file:
```bash
sudo nano /boot/firmware/config.txt
```

Add the following lines:
```
enable_uart=1
dtoverlay=disable-bt
```

The first line enables the primary UART (PL011) on GPIO14 (pin 8, TX) and GPIO15 (pin 10, RX). The second line disables Bluetooth to free up the UART for your use.

## 2. Disable Serial Console

Stop and disable the serial console service:
```bash
sudo systemctl stop serial-getty@ttyAMA0.service
sudo systemctl disable serial-getty@ttyAMA0.service
```

Edit cmdline.txt to remove console output:
```bash
sudo nano /boot/firmware/cmdline.txt
```

Remove `console=serial0,115200` or `console=ttyAMA0,115200` if present.

## 3. Hardware Connections

### PMS7003 Sensor
- **VCC** → 5V pin on Raspberry Pi
- **GND** → GND pin on Raspberry Pi
- **TX** → RX on USB-to-UART adapter
- **USB-to-UART** → Upper USB 2.0 port on Raspberry Pi (will appear as `/dev/ttyUSB0`)

### SIM Module
- **RX** → Pin 8 (GPIO14, UART TX) on Raspberry Pi
- **TX** → Pin 10 (GPIO15, UART RX) on Raspberry Pi
- **VCC** → 5V output from LM2596 module
- **GND** → GND from LM2596 module

### Pixhawk Connection
- **TELEM port** → USB-to-UART adapter
- **USB-to-UART** → Lower USB 2.0 port on Raspberry Pi (will appear as `/dev/ttyUSB1`)

## 4. Reboot

Apply changes:
```bash
sudo reboot
```

## 5. Verify UART

After reboot, verify the devices:
```bash
ls -l /dev/tty*
```

You should see the built-in UART and two USB serial devices ready for communication.

**Note on Built-in UART device name**: The device name varies by OS. On Ubuntu, it's `/dev/ttyAMA0`. On Raspberry Pi OS, it may appear as `/dev/serial0` or `/dev/ttyAMA0` if Bluetooth is disabled. This guide uses `/dev/ttyAMA0` for Ubuntu.

**Code Configuration**: Device paths are hardcoded in `src/device_setup.h`:
```c
#define UART0_FILE_PATH    "/dev/ttyAMA0"
#define USB0_FILE_PATH     "/dev/ttyUSB0"
#define USB1_FILE_PATH     "/dev/ttyUSB1"
```
If using a different OS, update `UART0_FILE_PATH` accordingly (e.g., `/dev/serial0` for Raspberry Pi OS).