
### **🛠 Project Overview**

Title: IoT-Based Room Electricity Control System using Person Counting and Motion Detection

Description: 
This project focuses on the design and implementation of an intelligent IoT system aimed at optimizing electricity consumption within a room through a dual-sensor approach. By combining Ultrasonic sensors for bidirectional person counting and PIR sensors for motion detection, the system creates a reliable automation logic that ensures power is only active when the room is truly occupied. This data is visualized using Blynk, transforming raw sensor inputs into a real-time mobile dashboard that displays live occupancy counts, motion status alerts, and a remote relay override, effectively bridging the gap between physical hardware automation and user-friendly cloud monitoring.

<br>

### **🛠 Prerequisites**

Software Requirements
1. Arduino IDE (v2.3.7): For firmware development and flashing the ESP32.
2. Blynk IoT: Cloud platform for remote control and data visualization (blynk.io).


Hardware Components
1. ESP32 Dev Board
2. Ultrasonic HC-SR04
3. PIR Motion Sensor
4. Relay Module (1-Ch)
5. LED Bulb & Fitting
5. Breadboard & Jumper Wires

<br>

### 🛠 **How to Practice**

1. Hardware Assembly Assemble the components on the breadboard following the wiring logic provided in the "Technical Documentation.pdf".

2. Blynk IoT Cloud Setup
- Log in to Blynk.io and create a New Template.
- Configure the Datastreams (Virtual Pins) as specified in the "Technical Documentation.pdf".

3. Open Arduino IDE v2.3.7.
- Install the Blynk and ESP32 board libraries via the Library Manager.
- Open the "arduino_code.ino" file from the src folder of this repository.
- Replace the placeholders with your credentials:

    ```
    C++
    char auth[] = "Your_Blynk_Auth_Token";
    char ssid[] = "Your_WiFi_Name";
    char pass[] = "Your_WiFi_Password";
    ```

4. Select DOIT ESP32 DEVKIT V1 as your board and click Upload.
