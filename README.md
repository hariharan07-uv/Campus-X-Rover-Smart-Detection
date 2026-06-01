# 🚀 Campus X Rover – Smart Detection Rover


## Overview

Campus X Rover is an intelligent embedded monitoring dashboard developed for a smart gas leak detection rover. The system utilizes an Arduino Uno, ILI9341 TFT display, ESP32 RFID communication module, and buzzer feedback system to provide a futuristic monitoring interface.

The dashboard displays gas sensor readings, environmental parameters, RFID access notifications, temperature trends, noise monitoring, and animated system diagnostics through an interactive graphical user interface.

This project demonstrates embedded GUI design, serial communication, robotics monitoring, and real-time visualization techniques.

---

# Features

## Advanced TFT Dashboard

* Futuristic User Interface
* Multi-Panel Layout
* Animated Gauges
* Real-Time Visual Feedback
* Embedded HMI Design

## RFID Integration

* RFID Card Detection
* ESP32 Serial Communication
* RFID Event Notifications
* Authentication Status Display
* Access Logging

## Environmental Monitoring

Simulated monitoring of:

* MQ-4 Methane Detection
* MQ-7 Carbon Monoxide Detection
* MQ-131 Ozone Detection
* Ambient Noise Level
* Temperature Monitoring

## Interactive Graphics

* Circular Gauges
* Arc-Based Indicators
* Animated Startup Sequence
* Temperature Trend Graph
* Dynamic Status Panels

## Audio Feedback

* RFID Detection Alerts
* Startup Sounds
* Status Notifications
* Event-Based Beeping

---

# Hardware Components

| Component           | Quantity    |
| ------------------- | ----------- |
| Arduino Uno         | 1           |
| ESP32               | 1           |
| ILI9341 TFT Display | 1           |
| RFID Reader Module  | 1           |
| Buzzer              | 1           |
| Jumper Wires        | As Required |

---

# System Architecture

RFID Card
↓
ESP32
↓
Serial Communication
↓
Arduino Uno
↓
TFT Dashboard
↓
Visual & Audio Notification

Environmental Data
↓
Dashboard Visualization
↓
Graphs & Gauges

---

# Hardware Connections

## ILI9341 TFT Display

| TFT Pin | Arduino |
| ------- | ------- |
| CS      | D10     |
| DC      | D9      |
| RST     | D8      |
| MOSI    | D11     |
| MISO    | D12     |
| SCK     | D13     |
| VCC     | 5V      |
| GND     | GND     |

---

## ESP32 Communication

| ESP32 | Arduino Uno |
| ----- | ----------- |
| TX    | Pin 2 (RX)  |
| RX    | Pin 3 (TX)  |
| GND   | GND         |

Baud Rate:

9600

---

## Buzzer

| Buzzer   | Arduino |
| -------- | ------- |
| Positive | Pin 6   |
| Negative | GND     |

---

# Dashboard Modules

## Gas Detection Panel

Monitors:

* MQ-4 Sensor
* MQ-7 Sensor
* MQ-131 Sensor

Features:

* Circular Gauge Indicators
* Color-Based Alerts
* Percentage Visualization

---

## Noise Monitoring

Displays:

* Ambient Noise Level
* Decibel Visualization
* Dynamic Color Indicators

---

## Temperature Monitoring

Displays:

* Celsius Reading
* Fahrenheit Reading
* Environmental Status

---

## Temperature Graph

Features:

* Historical Trend Analysis
* Real-Time Updates
* Dynamic Plotting

---

## RFID Status Panel

Displays:

* READY
* SCAN
* OK

Provides visual confirmation of RFID card detection and authentication events.

---

# Startup Animation

The system includes a cinematic startup sequence:

### Stage 1

Animated Particle Convergence

### Stage 2

Campus X Branding

### Stage 3

Rover Introduction

### Stage 4

Gas Leak Detection Module Presentation

### Stage 5

Interactive Welcome Screen

### Stage 6

Dashboard Launch

---

# RFID Communication Protocol

ESP32 sends:

```text id="n5wz3m"
RFID:AA:BB:CC:DD
```

or

```text id="ab8j4n"
AABBCCDD
```

Arduino responds:

```text id="vkt7jv"
ACK
```

---

# Software Libraries

Required Libraries:

* Adafruit_GFX
* Adafruit_ILI9341
* SPI
* SoftwareSerial

---

# Installation

## Step 1

Install required Arduino libraries.

## Step 2

Connect TFT display.

## Step 3

Connect ESP32 communication lines.

## Step 4

Connect RFID module to ESP32.

## Step 5

Upload firmware to Arduino Uno.

## Step 6

Power the system.

## Step 7

Present RFID card for authentication.

---

# Applications

* Smart Rover Dashboard
* Gas Leak Detection Systems
* Industrial Monitoring
* Robotics HMI
* Security Access Systems
* Smart Surveillance Platforms
* Environmental Monitoring
* Embedded GUI Research

---

# Advantages

✔ Professional Embedded UI

✔ RFID Authentication

✔ TFT Graphics Interface

✔ Audio Feedback System

✔ Serial Communication

✔ Robotics Integration

✔ Expandable Architecture

✔ Real-Time Visualization

---

# Future Improvements

* Actual Gas Sensor Integration
* WiFi Connectivity
* Cloud Dashboard
* Firebase Data Logging
* GPS Tracking
* Voice Alerts
* Touchscreen Interface
* AI-Based Hazard Detection
* Camera Integration
* Autonomous Navigation

---

# Project Highlights

✔ Arduino + ESP32 Hybrid System

✔ RFID Authentication

✔ TFT Graphics Dashboard

✔ Embedded HMI Development

✔ Real-Time Monitoring

✔ Robotics Interface

✔ Advanced UI Animations

✔ Industrial Design Approach

---

# Author

Hariharan Balakrishnan

B.E Electronics and Communication Engineering

Embedded Systems | Robotics | IoT | Edge AI | ESP32 | Zephyr RTOS

