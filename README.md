<img width="160" height="160" alt="232096427-removebg-preview" src="https://github.com/user-attachments/assets/5dab2d59-6904-4110-a1c2-1896bf10f551" />

HIZMOS Firmware (Beta 1.3)

![Beta](https://img.shields.io/badge/Status-Beta%201.3-orange)
![Status](https://img.shields.io/badge/Status-Beta-orange)
![Target](https://img.shields.io/badge/Target-ESP32--S3-green)
![License](https://img.shields.io/badge/License-Apache%202.0-blue)
![GitHub Stars](https://img.shields.io/github/stars/Hiktron/Hizmos)
![GitHub Forks](https://img.shields.io/github/forks/Hiktron/Hizmos)
![Contributors](https://img.shields.io/github/contributors/Hiktron/Hizmos)
![Platform](https://img.shields.io/badge/Platform-HIZMOS-red)

Overview

HIZMOS is a modular embedded firmware platform built for ESP32-S3-based hardware systems, focused on:

RF experimentation (sub-GHz + 2.4GHz)
NFC / RFID research
BLE + USB HID automation
Embedded signal tools (oscilloscope / generator)
Educational cybersecurity and hardware development

This firmware is currently in Beta 1.3, actively evolving and not production stable.

Officially Supported Targets
Target	Status	Notes
ESP32-S3	Main Development	Fully supported
ESP32-P4	Experimental	firmware_p4 branch
ESP32-C5	Experimental	early bring-up
Firmware Architecture

Unlike single-file Arduino projects, HIZMOS uses a component-based ESP-IDF-style architecture:

components/
├── Drivers/        → Hardware abstraction layer (RF, NFC, GPIO, Display)
├── Services/       → System services (BLE, storage, logging, OTA)
├── Core/           → Kernel logic, task manager, scheduler
├── Applications/   → User-facing modules (tools, utilities, apps)
Design Goals
Fully modular firmware (plug-in architecture ready)
Hardware abstraction per module
Independent app execution layer
Multi-protocol communication support
System Overview
Core Features
BLE HID (Keyboard / Mouse / Custom GATT services)
USB OTG HID + CDC bridge
Wi-Fi networking (TCP / UDP / control API)
Sub-GHz RF communication (CC1101)
2.4GHz communication (NRF24 / GT24 modules)
NFC (PN532-based stack integration)
IR transmit/receive subsystem
Signal & RF Tooling
RF Modules
CC1101 sub-GHz transceiver
NRF24 / GT24 2.4GHz communication layer
Capabilities (research use only)
Signal sniffing
Packet replay testing
Device communication emulation (lab environments)
Channel scanning and diagnostics
NFC / RFID Stack
ISO14443A / 14443B support
ISO15693 (NFC-V) compatibility
NDEF read/write operations
Card profiling tools
Low-level sector/block access (hardware dependent)
Signal Tools
Digital/analog oscilloscope (basic sampling mode)
PWM / waveform generator
Frequency measurement tools
Serial real-time stream analyzer
System Tools
File system browser (SD card / flash)
EEPROM viewer
Serial monitor bridge
Firmware OTA updater
Debug logging console
Communication Interfaces
BLE GATT (primary control channel)
USB CDC (debug + data streaming)
USB HID (input emulation)
Wi-Fi TCP/UDP (remote control + telemetry)
Repository Structure
HIZMOS/
├── firmware/
│   ├── components/
│   ├── main/
│   └── CMakeLists.txt
├── tools/
│   ├── pc_app/
│   └── python_bridge/
├── docs/
├── hardware/
└── README.md
Default Pin Mapping (ESP32-S3)
SPI Devices (RF modules)
SCK  -> GPIO12
MISO -> GPIO13
MOSI -> GPIO11
CS   -> GPIO10
IRQ  -> GPIO4
I2C Devices (NFC / sensors)
SDA -> GPIO8
SCL -> GPIO9
IRQ -> GPIO4
Development Philosophy

HIZMOS is designed as:

A research platform
A hardware experimentation toolkit
A modular embedded OS-like firmware
A learning environment for RF + embedded systems

Not a closed product — but a hackable system stack.

How to Build
ESP-IDF Setup

Recommended:

ESP-IDF v5.x+
CMake build system
idf.py set-target esp32s3
idf.py build
idf.py flash
Contributing

We follow a structured contribution model:

Branching
main        → stable releases
beta        → active development (Beta 1.3)
feature/*   → new modules
Commit Style
feat(core): add task scheduler
fix(rf): correct cc1101 timing bug
add(nfc): iso15693 read support
Roadmap
Beta 1.3
Modular firmware stabilization
BLE HID improvements
CC1101 driver upgrade
NFC read/write expansion
Beta 2.0
Plugin-based app system
PC companion full integration
UI rewrite (graphics + menu system)
1.0 Release
Stable API layer
Documentation freeze
Public hardware revision support
License

GPLv3 — open for modification and redistribution under same license terms.

Status
Project: HIZMOS Firmware
Stage: Beta 1.3 (Active Development)
Stability: Experimental
Target: ESP32-S3 ecosystem

![ce98e05e-ab88-439e-80e3-fdce7ed647ef](https://github.com/user-attachments/assets/2637c95d-003a-45a1-95ac-b6e2210f10bc)
<img width="803" height="710" alt="image" src="https://github.com/user-attachments/assets/9490e63d-0ef2-4b6a-aa0e-8bc1ead2c588" />


📡 Infrared (IR)

❌ Universal Remotes (Work in progress)

✅ Read Remotes

✅ Save Remotes

💻 BadUSB

✅ Demo Payload

✅ Keyboard Emulation

✅ HID Script Execution

📶 NRF Tools

✅ Signal Analyzer

❌ Hijack Nerf Devices (Work in progress)

✅ 2.4GHz Jammer

✅ Jammer (Specific Channel)

🗂️ File Manager

✅ Browse Files

✅ Rename Files

✅ Delete Files

🎮 Mini Apps

✅ Basic Flipper-like Apps

⚙️ Settings

✅ Show Usage

✅ Format SD Card

✅ Restart Device

❌ Battery Info (Work in progress)

✅ SD Card Info

✅ About

✅ Check Connected Devices

🧩 Not Yet Implemented

🔲 NFC

🔲 Sub-GHz

🔲 GPIO Tools

🔲 Wi-Fi Tools

🔲 BLE Tools


Contact us on: https://www.instagram.com/hiktron/

whatsapp: +201557322422

help us to build more projects: https://www.patreon.com/c/HIKTRON

get all new on : https://www.youtube.com/@HIKTRON
