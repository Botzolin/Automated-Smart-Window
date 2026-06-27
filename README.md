# Automatic-Window-Opener
University project developed for the **Electric Machines and Drives (MEA)** course.
 
## Description
 
IoT system for automatically controlling a window based on ambient temperature. The system reads temperature with a DHT11 sensor, decides whether to open/close the window via a linear actuator (BTS7960 driver), and exposes monitoring + manual control through a Node-RED dashboard, with communication handled over the MQTT protocol.
 
**Hardware components:**
- Arduino UNO R4 WIFI
- DHT11 temperature/humidity sensor
- Linear actuator + BTS7960 motor driver
- MQTT broker (laptop)
- Node-RED dashboard
**Main features:**
- real-time temperature monitoring
- automatic mode: window opens/closes based on a temperature threshold
- manual mode: control from the dashboard
- data transmission via MQTT
- Node-RED graphical interface
## Repository contents
 
| File | Description |
|---|---|
| `Proiect_MEA.ino` | Arduino source code (Arduino IDE) — DHT11 reading, actuator control (PWM via BTS7960), WiFi connection, MQTT client (publishes temperature, subscribes to AUTO/MANUAL/OPEN/CLOSE commands) |
| `Referat_MEA_Fereastra_Smart.pdf` | Project report — objectives, solution description, system architecture, testing, electrical schematic and Node-RED interface |
| `Demonstratie_Proiect_Fereastra_Smart.mp4` | Demo video showing the system in operation |
 
## Team
 
- **Moise Cristian** — mechanical structure of the window, wooden frame, actuator mounting system, contributed to Arduino software
- **Ilinca Tudor-Ioan** — Node-RED interface, MQTT communication between dashboard and Arduino, electrical wiring
- **Macaneata-Vamos Andrei** — Arduino application development, project coordination, software architecture and electrical schematic
## Status
 
Academic project, completed and tested (sensor, actuator, automatic mode, MQTT communication, graphical interface).
