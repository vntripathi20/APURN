# APURN

### Arduino-Based Portable Unsafe Food Recognition Node

APURN is a portable embedded system designed to detect potential spoilage in sealed canned foods using low-cost gas sensors and microcontroller-based analysis. The device checks volatile gas levels associated with food decomposition and alerts the user through a mobile application made in MIT App Inventor and a physical LCD screen.

I try to explore how affordable electronics and environmental sensing can be used to improve everyday food safety and reduce the risk of foodborne illness.

## Motivation

Foodborne illnesses remain a significant global health concern. Certain bacteria, such as those responsible for severe food poisoning, can produce toxins in improperly stored or damaged canned foods. These toxins are often undetectable by sight or smell, making it difficult for consumers to determine whether food is safe before opening it. With this uncertainty also comes a lot of food waste, which I believe there must be a solution for.

APURN was created to investigate whether inexpensive sensors and embedded systems could provide an additional warning system for potential spoilage.

## Features

Gas detection using low-cost air quality sensors (MQ-3 and MQ-9)

Arduino-based embedded processing

Wireless communication via Bluetooth (HC-05 module)

LCD display for real-time status updates

Mobile application interface for reading and interpreting sensor data, making it more convenient for consumers

Portable hardware prototype

System Architecture

## APURN consists of three main subsystems:

1. Sensor System

Gas sensors detect volatile compounds that may be produced during food decomposition. These sensors continuously monitor the surrounding air for chemical changes.

2. Microcontroller Processing

An Arduino microcontroller collects sensor readings and processes the data. Based on predefined thresholds or patterns, the system determines whether the readings indicate possible spoilage.

3. User Interface

The system communicates results through:

OLED display for quick on-device feedback

Bluetooth connection to a smartphone

Mobile application built with MIT App Inventor

Hardware Components

Arduino microcontroller

MQ-series gas sensor(s)

HC-05 Bluetooth module

LCD display

Breadboard prototype wiring

Power supply (battery or USB)

Future iterations may include a custom KiCad-designed PCB shield to simplify hardware integration.

Software Stack

Embedded Software

Arduino C/C++

Sensor data acquisition and processing

Serial and Bluetooth communication

Mobile Application

MIT App Inventor

Bluetooth data reception

Status display for the user

## How It Works

The user places APURN near a sealed canned food item.

Gas sensors measure airborne compounds that may indicate decomposition.

The Arduino reads and processes sensor data.

Sensor readings are transmitted via Bluetooth.

The LCD display and mobile app show a status message.

The goal is to give users an early indication that food may be unsafe.

Current Development Status

### APURN is currently a prototype research project focused on testing the feasibility of detecting food spoilage signals using low-cost gas sensors.

The system successfully demonstrates:

sensor data acquisition

wireless data transmission

real-time user feedback

Further testing and calibration are required for real-world deployment.

## Future Improvements

Planned improvements for future versions include:

Improved gas sensor selection and calibration

Custom PCB shield designed in KiCad

Machine learning-based pattern analysis

Expanded mobile application features

Cloud-based data logging

Additional environmental sensors

Testing with different food types
