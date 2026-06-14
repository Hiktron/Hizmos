/*





# HIZMOS Journey


The original goal was simple: create a hardware hacking and STEM device inspired by devices like Flipper Zero,
but at a much lower price and with more powerful hardware and having every single feature i have dreamed for

## Why I Started HIZMOS

I wanted to create a device that students, makers, and hardware enthusiasts could actually get.
many existing devices are expensive or difficult to obtain in my region,
so I started designing a platform that could combine wireless experimentation
, hardware development, electronics tools, and educational features into a single device.
hizmos started as a very simple project but now
it is a very unique and cool device combining the powerful software, hardware affordabilty and opensource 


## What did i add during the last 23 hours in the horizon project

Apps:
Recreating the full apps menu to be horizontal instead of vertical

Freq Finder app : the user enters the the frequency and hizmos tells what is it used for 
    example: 433mhz -> used in car remotes (ism band)
<img width="744" height="550" alt="image" src="https://github.com/user-attachments/assets/b75d9d5a-9dd2-4cf8-af27-4110006330e4" />


## Four Major Challenges (through the whole hizmos journey)

### 1. Sourcing Components in Egypt

One of the biggest challenges was finding the required components and modules. 
Some parts were difficult to obtain locally,
getting the ultimate components and tools to make the best tool ever was a real problem
while importing parts increased costs and delivery times and also the customs clearance was a huge problem



### 2. SPI busses config

Combining an ESP32-S3,2x CC1101 (sub-ghz), 2x nrf24 2.4ghz , Sd card, and expansion interfaces 
all of those works on spi bus required many design revisions and consumed alot of effort 
to get the pcb design right from the first time

### 3. features implemntation

Building firmware for multiple features at the same time was difficult.
Every new capability required testing and a certain config so it wouldn't conflict with any other feature or hardware.
debugging, drivers and libs editing, and optimization to make everything work together reliably.

### 4. drivers 
every time i need to add a new feature or config something in 
the hardware you have a problem with a very low level hardware driver , datasheets and registers 
dealing with stuff like the st25r3916, cc1101 , ble tools
all of those needs a custom drivers to do the function for hizmos and now we are working on them



## What Motivated Me

### 1. The Maker and Open-Source Community

Every time i see somebody trying hizmos or making a video about it it was wonderful for me 
everytime somebody forks hizmos or comment i think a idid something great and appreciatable 

### 2. Industry Support and Sponsorship

NextPCB sponsored the manufacturing of 10 double-layer PCB prototypes,
allowing me to test, iterate, and improve the hardware design much faster than would have been possible otherwise.
Having access to professionally manufactured PCBs was a significant step toward turning HIZMOS from a concept 
into real hardware.

ELECHOUSE sponsored both the PN5180 and ST25R3916 NFC modules.
These are among the most advanced NFC MODULES available for makers and embedded developers
Access to these modules enabled me to explore advanced NFC functionality and integrate professional-grade NFC features into HIZMOS.


## Where HIZMOS Is Today:



Today, HIZMOS has evolved from an idea into a working platform. Multiple hardware and software components have been designed, tested, and improved over time.

## Current Features
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

✅know the software version 

✅ Format SD Card

✅ Restart Device

❌ Battery Info (Work in progress)

✅ SD Card Info

✅ About

✅ Check Connected Devices

📶 Sub-GHz

✅read raw

✅spectrum graph

✅freq analyzer
 
✅jammer

✅modulation config


🧩 Not Yet Implemented

🔲 NFC (waiting for Drivers Dev)

🔲 GPIO Tools

🔲 Wi-Fi Tools (only the attacks left)

🔲 BLE Tools



## Looking Forward

The project is still evolving. Future work includes improving hardware reliability,
expanding software capabilities, refining the user interface,
and growing the open-source ecosystem around HIZMOS.

HIZMOS started as an idea for a cheaper alternative,
but it has grown into a much larger learning experience involving hardware design, 
embedded programming, problem solving, and product development.





















*/
