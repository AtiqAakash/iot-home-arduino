# IoT Multipurpose Smart Home/Office/Industrial Automation/Monitoring System

## Overview

This project implements a versatile IoT-based monitoring and automation system that enhances security and provides real-time environmental monitoring. It is designed to be deployed in homes, offices, and industrial settings to monitor and control various parameters such as motion, temperature, humidity, flame, and gas presence. The system utilizes sensors and a web-based interface for remote monitoring and control from any platform.

## How It Works

The system operates with a NodeMCU (ESP8266) microcontroller connected to various sensors including PIR motion, DHT22 for temperature and humidity, MQ5 for gas detection, and IR flame sensor. These sensors continuously monitor their respective environmental conditions.

The NodeMCU communicates this data to a central server over WiFi. Users can access real-time sensor data and control system parameters through a browser-based interface. The system can also trigger actions such as sending notifications (email, SMS, phone calls) and activating alarms based on predefined thresholds or detected anomalies.

## Necessity and Use Cases

### Home Automation
- **Security**: Detects unauthorized motion or intrusion with PIR sensors and notifies homeowners immediately.
- **Fire Safety**: Alerts occupants in case of fire with the IR flame sensor, allowing timely response and evacuation.
- **Gas Monitoring**: Monitors gas levels to prevent leaks and ensure safety.

### Office Automation
- **Environment Monitoring**: Maintains optimal office conditions by monitoring temperature and humidity levels.
- **Security**: Detects unauthorized access using motion sensors and notifies security personnel.
- **Emergency Response**: Provides early warning of fire incidents and facilitates prompt evacuation.

### Industrial Automation
- **Safety Compliance**: Ensures workplace safety by monitoring hazardous conditions such as gas leaks and flame detection.
- **Process Monitoring**: Monitors environmental parameters critical to industrial processes for efficiency and safety.
- **Remote Management**: Allows remote monitoring and control of critical systems, reducing downtime and improving operational efficiency.

## Components 

- **NodeMCU**: ESP8266-based microcontroller for WiFi connectivity.
- **Arducam OV2640**: Camera module for image capture.
- **MQ5**: Gas sensor for detecting gas presence.
- **IR Flame Sensor**: Infrared sensor for flame detection.
- **DHT22**: Temperature and humidity sensor.
- **PIR Motion Sensor**: Passive Infrared (PIR) sensor for motion detection.
- **SW-420**: Vibration sensor for detecting vibrations.
- **Sim900A mini v4**: GSM module for making phone calls and sending SMS.

## Interface

![System Interface](interface1.png)

## Technologies

- **Programming Languages**: C++, HTML, CSS, Javascript
- **Frameworks**: Bootstrap for responsive design
- **Integration**: IFTTT for event-based notifications and automation

## Awards

- **2nd Prize**: Intra University Project Competition - 2018, University of Development Alternative (UODA)
  <img src="Prize.jpg" alt="2nd Prize" width="300">

## Contact

For more information or inquiries about this project, please contact Mohammad Atiqur Rahman Aakash at aakash.unipune@gmail.com.
