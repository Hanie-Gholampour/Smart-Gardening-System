# Smart-Gardening-System

## Overview
This project monitors temperature and humidity in a garden using IoT and provides remote control for gardening tools like a servo motor and NeoPixel LEDs.

## Features
- Real-time temperature and humidity monitoring.
- Remote control via a Node-RED dashboard.
- Data logging and customizable lighting.

## Components
- ESP32 microcontroller.
- DHT22 temperature and humidity sensor.
- Servo motor for automated tasks.
- NeoPixel LEDs for lighting customization.
- LED Switch for ensuring the connection sustainability.

## Files
- `SmartGardeningSystem.ino`: Arduino source code for the ESP32.
- `NodeRED_Dashboard.json`: Node-RED flow file.
- `SmartGardeningSystem_Presentation.pptx`: Node-RED flow file.

## How to Use
1. Import the `NodeRED_Dashboard.json` into Node-RED.
2. Upload the `SmartGardeningSystem.ino` to the ESP32 using Arduino IDE or Wokwi.
3. Use the Node-RED dashboard to monitor and control the system.

## Demo
[Wokwi Simulation Link](https://wokwi.com/projects/417080819617940481)
