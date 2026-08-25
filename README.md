<img width="160" height="160" alt="232096427-removebg-preview" src="https://github.com/user-attachments/assets/5dab2d59-6904-4110-a1c2-1896bf10f551" />


# HIZMOS Journey


the original goal was simple is to create a hardware hacking and rf device inspired by devices Flipper Zero
but at a much lower price and with more powerful hardware and having every single feature i have dreamed for

## why I started hizmos

I wanted to create a device that students, makers, and hardware guys could afford
many existing devices are expensive or difficult to get in my region,
so I started designing a platform that could combine wireless experimentation
, hardware development, electronics tools, and educational features into a single device.
hizmos started as a very simple project but now
it is a very unique and cool device mixing the powerful software, hardware affordabilty and opensource 


hizmos firmware (Beta 1.3)

![Beta](https://img.shields.io/badge/Status-Beta%201.3-orange)
![Status](https://img.shields.io/badge/Status-Beta-orange)
![Target](https://img.shields.io/badge/Target-ESP32--S3-green)
![License](https://img.shields.io/badge/License-Apache%202.0-blue)
![GitHub Stars](https://img.shields.io/github/stars/Hiktron/Hizmos)
![GitHub Forks](https://img.shields.io/github/forks/Hiktron/Hizmos)
![Contributors](https://img.shields.io/github/contributors/Hiktron/Hizmos)
![Platform](https://img.shields.io/badge/Platform-HIZMOS-red)

## overview

This repository contains firmware in development for the hizmos 
Warning: this firmware is in its beta phase and is still in the development process

---

## Supported dev board

We are using the latest Espressif chips:

| Target      | Status |
|------------|--------|
| ESP32-S3   | Main Development |


---
## Hardware bom

| Module      | Use |
|------------|--------|
| ESP32 S3   | Main MCU |
| i2c OLED 12864 | THE MAIN DISPLAY |
| 2X NRF24L01 | 2.4GHZ TRANSCEIVER |
| ir led & RECEIVER |IR REMOTE |
| CC1101 | SUBGHZ TRANSCEIVER  |
| SPI SD CARD module | sd card  |



![ce98e05e-ab88-439e-80e3-fdce7ed647ef](https://github.com/user-attachments/assets/2637c95d-003a-45a1-95ac-b6e2210f10bc)
<img width="803" height="710" alt="image" src="https://github.com/user-attachments/assets/9490e63d-0ef2-4b6a-aa0e-8bc1ead2c588" />


## Complete Connection Diagram

```text
ESP32-S3
│
├── I2C
│   ├── SDA  → GPIO8
│   └── SCL  → GPIO9
│
├── IR
│   ├── TX   → GPIO35
│   └── RX   → GPIO36
│
├── nRF24 #1
│   ├── CE   → GPIO10
│   ├── CSN  → GPIO11
│   ├── SCK  → GPIO18
│   ├── MISO → GPIO16
│   └── MOSI → GPIO17
│
├── nRF24 #2
│   ├── CE   → GPIO12
│   ├── CSN  → GPIO13
│   ├── SCK  → GPIO18
│   ├── MISO → GPIO16
│   └── MOSI → GPIO17
│
├── CC1101 #1
│   ├── CS   → GPIO45
│   ├── GDO0 → GPIO21
│   ├── GDO2 → GPIO47
│   ├── SCK  → GPIO15
│   ├── MISO → GPIO3
│   └── MOSI → GPIO46
│
├── CC1101 #2
│   ├── CS   → GPIO40
│   ├── GDO0 → GPIO41
│   ├── GDO2 → GPIO42
│   ├── SCK  → GPIO15
│   ├── MISO → GPIO3
│   └── MOSI → GPIO46
│
├── SD Card
│   ├── CS   → GPIO37
│   ├── SCK  → GPIO14
│   ├── MISO → GPIO39
│   └── MOSI → GPIO38
│
└── Buttons
    ├── UP     → GPIO4
    ├── DOWN   → GPIO5
    ├── SELECT → GPIO6
    ├── BACK   → GPIO7
    ├── LEFT   → GPIO1
    └── RIGHT  → GPIO2

```

# Flashing ESP32-S3 Firmware with ESP Web Tool

Use the following files and flash addresses:

| Address | File |
|----------|----------|
| 0x0000 | HIZMOS_OLED_U8G2lib.ino.bootloader.bin |
| 0x8000 | HIZMOS_OLED_U8G2lib.ino.partitions.bin |
| 0x10000 | HIZMOS_OLED_U8G2lib.ino.bin |

## Steps

1. open https://esptool.spacehuhn.com/ or any other web flasher tool
2. Click **Connect** and select your ESP32-S3 serial port
3. Click **Add File** and add:
   - `HIZMOS_OLED_U8G2lib.ino.bootloader.bin` at address `0x0000`
   - `HIZMOS_OLED_U8G2lib.ino.partitions.bin` at address `0x8000`
   - `HIZMOS_OLED_U8G2lib.ino.bin` at address `0x10000`
4. Enable **Erase Before Flashing**
5. click **Program**
6. after flashing reboot the board

## Notes


- Target chip: **ESP32-S3(N16R8)**

SubGhz: Spectrum graph, jammer, brute force, read, read raw, frequency analyzer

Infrared: Read remotes, save signals

BadUSB: Demo payload, keyboard emulation, HID script execution

NRF Tools: Signal analyzer, 2.4GHz jammer (specific channel), packet counter

File Manager: Browse, rename, delete files on SD card

Mini Apps: Calculator, Timer, Snake game, SD flasher, Flashlight, Academy, freq finder, safe mode

Settings: Show Usage, Format SD Card, Restart Device, SD Card Info, About, Check Connected Devices

Contact us on: https://www.instagram.com/hiktron/

get all new on : https://www.youtube.com/@HIKTRON
