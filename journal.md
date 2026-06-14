/*

# Note : 
### if u are here for trying hizmos this file is not for u this file is for horizon hackclub reviewers and u will not get any benifit from reading it just neglige it





# HIZMOS Journey


the original goal was simple is to create a hardware hacking and rf device inspired by devices Flipper Zero
but at a much lower price and with more powerful hardware and having every single feature i have dreamed for

## why I started HIZMOS

I wanted to create a device that students, makers, and hardware guys could afford
many existing devices are expensive or difficult to get in my region,
so I started designing a platform that could combine wireless experimentation
, hardware development, electronics tools, and educational features into a single device.
hizmos started as a very simple project but now
it is a very unique and cool device mixing the powerful software, hardware affordabilty and opensource 


# What did i add during the last 23 hours in the horizon project

### Apps:
Recreating the full apps menu to be horizontal instead of vertical and all the icons are saved in icons.h file to make the code much cleaner

<img width="800" height="500" alt="image" src="https://github.com/user-attachments/assets/85853544-f1fe-4a5f-95fc-d31302c0c68f" />



### Freq Finder app :
the user enters the the frequency and hizmos tells what is it used for 
example: 433mhz -> used in car remotes (ism band)
<img width="496" height="367" alt="image" src="https://github.com/user-attachments/assets/b75d9d5a-9dd2-4cf8-af27-4110006330e4" />


### Safe mode :
the use can switch to it to use hizmos sfely 100% it doeasnt have any rf transimission or any type of jamming
<img width="785" height="390" alt="image" src="https://github.com/user-attachments/assets/097415f4-92aa-4bfd-b87d-0474bd54d87a" />


### flashlight app : 
using the neopixel led at hizmos to be used as a flashlight and the colors could be configuered 

<img width="440" height="400" alt="image" src="https://github.com/user-attachments/assets/d912f0e7-f5b7-4d28-a891-d5989f14f9d2" />

### academy app: 
the user can text his skills to get xp it has 3 difficulties (easy /meduim / Hard) to test 3 main skills or tracks (Rf / embedded systems / hardware pentesting) and the questions are also implemented through db.cpp/db.h files

<img width="750" height="490" alt="image" src="https://github.com/user-attachments/assets/d47cbd32-475c-4f21-83f3-ddf78660b27f" />


## 2.4ghz:

### Blue jammer (expirmental)
feature that uses the both nrf24 modules in hizmos to jam on the bluetooth full spectrum and to jam on the channel hopping in the bluetooth devices 

<img width="510" height="480" alt="image" src="https://github.com/user-attachments/assets/7f263d8a-4b83-4c56-824b-328ee6c5ac28" />


### Packets Counter

it counts the recived packets over the whole 2.4ghz spectrum and shows them

<img width="380" height="500" alt="image" src="https://github.com/user-attachments/assets/16ab884c-8101-4eaa-a8fa-ee5a0a4cd4a2" />



## Wifi

### Wifi Deauthintecation Attack (expirmental)

wifi deauthintecation attack is used to send the deauth packets to the connected devices on the router to disconnect them

<img width="369" height="450" alt="image" src="https://github.com/user-attachments/assets/28a501ec-1066-4bad-a9da-58b9f665cead" />



### captive portal :

it replicates a wifi ap so the users connect to it and on connecting it redicts them to a fake login page to collect the data
i have done 3 web pages (Instgram/ whatsapp / hotel login / mall of egypt free wifi)

<img width="335" height="301" alt="image" src="https://github.com/user-attachments/assets/9772af2f-bca0-4df5-b2c2-74c78a32d14f" /> <img width="500" height="270" alt="image" src="https://github.com/user-attachments/assets/bc66ddf9-8d9b-49b5-bfa5-1f807cab9078" />


## Ble

### Ble advertiser (expirmental)
it adverts fake ble devices around using some pre made packets
mainly for now the samsung watch

<img width="650" height="256" alt="image" src="https://github.com/user-attachments/assets/f57ac207-6f21-4604-887d-f51031864c38" />


## Main

- Fixing the button debouncing in some menus

- add the legal notice before entering the main


## What will be added at the rest of the time

Pn5180 Nfc Support (read/write/emulation)

Notfications system : get notifed when something happens like wifi connection or sd card inserting

rewards system : get rewarded when something happens like netwronk sniffing or get a high xp in the academy or in a game

subghz system : JAMMING , reading and remmiting the signals , saving to the sd card packet analysis

ir : jamming on the ir signal using noise , universal remote to control every single ir device using hizmos

5ghz : some 5ghz wifi scanning and expirmental attacks




# Bom

### Note the prices in this bom is the prices i have payed for those stuff for a single hizmos piece

esp32 s3 n16r8 1u (700 egp -> 14$ apx)

2x cc1101 (300 egp /piece -> 6$ apx ) allover is 12$

2x gt24 mini nrf24 (300 egp /piece -> 6$ apx ) allover is 12$

oled 1.3 i2c (350 egp -> 7$ apx)

pn532 nfc module (250 egp -> 5$ apx)

passive components including : (650 egp -> 13$ apx)

 sma connectors
 
 1206/ 0805 smd resistors and caps
 
 usb to ttl ch340
 
 tp4056 charging module
 
 some other stuff 

soldering (500 egp / 10$)

THe pcbs was a sponsorship so all i have payed was the customs (200 egp -> 4$ apx)


shipping cost : (1200 egp -> 24$ apx)

the overall apx . is 100$ -> 5000 egp

the rest of the hardware that not implemnted yet

pn5180 (sent by elechouse)

esp32 c5 (21$)

Ir & 433 mhz testing remotes (400 egp -> 8$ apx)



# note
the videoi have submitted only has the main functions not testing everything when i finnish the time i need to finish at hack club i will start doing the video testing every single feature and also i will make now a video on lapse to show the menus with the features logic and the stuff

the .json file of easy eda design is on one of my hard disks iam searching for it lets leave it for the next submit

but the gerber file is here on github



## Four Major Challenges (through the whole hizmos journey)

### 1. Sourcing Components in Egypt

One of the biggest challenges was finding the required components and modules. 
Some parts were difficult to obtain locally
getting the best components and tools to make the best tool ever was a real problem
while importing parts increased costs and delivery times and also the customs clearance was a huge problem



### 2. SPI busses config

Combining an ESP32-S3,2x CC1101 (sub-ghz), 2x nrf24 2.4ghz , Sd card, and expansion baords 
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

every time i see somebody trying hizmos or making a video about it it was wonderful for me 
everytime somebody forks hizmos or comment i think a idid something great and appreciatable 

### 2. Industry Support and Sponsorship

NextPCB sponsored the manufacturing of 10 double layer pcbs
allowing me to test and improve the hardware design much faster than would have been possible 
Having access to professionally manufactured PCBs was a significant step toward turning hizmos from a concept 
into real hardware.

ELECHOUSE sponsored both the PN5180 and ST25R3916 NFC modules.
These are among the most advanced NFC MODULES available for makers and embedded developers
Access to these modules enabled me to explore advanced NFC functionality and integrate professional-grade NFC features into HIZMOS.



## hizmos levels



### the very first hizmos on a breadboard using the nokia display

<img width="1024" height="1024" alt="image" src="https://github.com/user-attachments/assets/e3a71554-e4c4-46cd-a3c8-8f520bd7f24a" />

### using the oled and adding the cc1101 

<img width="1079" height="1068" alt="image" src="https://github.com/user-attachments/assets/079f3b3c-b432-4474-ab47-a92a6cb089e7" />

### the day when i got the pcbs from nextpcb

<img width="1600" height="1066" alt="image" src="https://github.com/user-attachments/assets/a4f04522-ea3a-4e90-b05c-416450e736d0" />

### hizmos now

<img width="3456" height="2304" alt="image" src="https://github.com/user-attachments/assets/5cee2c58-f038-451b-9ae1-add34b332b95" />




## Where HIZMOS Is Today:



Today hizmos has changed from an idea into a working platform. alot of hardware and software components have been designed, tested, and improved over time.

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
