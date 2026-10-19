# Dual-control-Smart-Switch
An IoT-based smart switching system that allows users to control an electrical appliance through a web interface and physical clap input. The ESP32 handles Wi-Fi communication, web requests, and relay control.

1. Project Overview

This project demonstrates how embedded systems, Wi-Fi connectivity, and web technologies can be integrated to create a smart switching solution.

The system uses an ESP32 microcontroller, a sound sensor, and a relay module to provide two control methods:

- **Web-based control:** Switch the connected output ON or OFF using a web browser on the same network.
- **Clap-based control:** Use a physical clap as an input to trigger switching, if enabled in the firmware.

2. Features

- Browser-based ON/OFF control
- ESP32 Wi-Fi connectivity
- Local web server for switch commands
- Sound sensor input for clap detection
- Relay-based switching
- Cooldown timer to help prevent repeated accidental triggers
- Automatic synchronization of switch status with the web interface, if implemented

3. Hardware Components

- ESP32 development board
- Sound sensor module
- 5V relay module
- Breadboard and jumper wires
- USB cable and compatible power source

4. Software and Technologies

- Arduino IDE
- Embedded C/C++ (Arduino framework)
- ESP32 Wi-Fi libraries
- HTTP-based web interface
- HTML/CSS/JavaScript

5. System Architecture

The system consists of three main stages:

1. **Inputs and Control:** A smartphone or computer browser sends commands through Wi-Fi. The sound sensor provides clap input.
2. **Processing:** The ESP32 runs the web server, handles input signals, and manages the switching logic.
3. **Outputs:** The ESP32 controls the relay module, which switches the connected load.

   System Flow:

      Browser → Wi-Fi Router → ESP32 → Relay Module → Electrical Load      
      Clap → Sound Sensor → ESP32 → Switching Logic

6. Getting Started

Prerequisites:

- Arduino IDE
- ESP32 board support package
- Compatible USB data cable
- Required ESP32 Wi-Fi libraries

Setup Instructions:

1. Clone or download this repository.
2. Open the Arduino sketch in Arduino IDE.
3. Install the required board support package and libraries.
4. Enter your Wi-Fi credentials in the appropriate configuration section.
5. Select the correct ESP32 board and COM port.
6. Upload the code to the ESP32.
7. Open the Serial Monitor to check the connection status and obtain the device IP address, if printed by the firmware.
8. Connect your browser device to the same network and open the displayed IP address.
9. Test the web switch and clap input.

**Note:** Update these instructions to match the actual Wi-Fi configuration and libraries used in your code.

7. Testing

The following behaviors can be tested:

- ESP32 connects successfully to Wi-Fi.
- The web interface loads on a compatible device.
- ON/OFF commands change the relay state.
- Clap input triggers switching as intended.
- The cooldown timer prevents unwanted repeated triggers.
- The displayed switch state matches the actual system state.

8. Safety Considerations

- Test the logic with a low-voltage load or a suitable indicator before connecting mains-powered equipment.
- Never handle exposed mains wiring while the circuit is powered.
- Use an appropriately rated, enclosed relay assembly and suitable electrical protection.
- Do not expose Wi-Fi passwords or other credentials in the repository.

9. Project Demonstration

  <img width="3849" height="2206" alt="Setup" src="https://github.com/user-attachments/assets/eb034bc4-22bd-4979-905f-5338bc9b1852" />
  <img width="2296" height="4080" alt="Hardware" src="https://github.com/user-attachments/assets/6cd8cbdb-1ba3-4070-b881-5f81b398fbb3" />
  <img width="4080" height="2296" alt="Code" src="https://github.com/user-attachments/assets/ad2a23b3-0267-4895-a0e8-20e8786606c9" />
  
  Suggested images:
  - Complete hardware and web interface
  - Hardware close-up
  - System architecture diagram

10. Author

CHAKKA SRIKEERTHI

Engineering Student | IoT | Embedded Systems | Cybersecurity
