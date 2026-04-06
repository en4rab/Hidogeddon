# Hidogeddon

Welcome to the **Hidogeddon** repository. This project is a specialised firmware implementation designed for interacting with **HID proximity cards and readers** but will work with any readers that provides wiegand output, similar to the Paxtogeddon reader but tailored for wiegand output.

---

## ⚠️ Important Attribution
**Credit where credit is due:** Please note that **100% of the firmware and technical development** found in this repository is the work of **Daniel**. 

I am simply hosting this project on my GitHub to ensure the code has a permanent home and remains accessible. I did not write this code; Daniel deserves all the credit for the reverse engineering, logic, and implementation of Hidogeddon.

---

## Project Overview

Hidogeddon is designed to run on the **same hardware as the Paxtogeddon reader**. While Paxtogeddon focuses on Paxton-based systems, this firmware adapts that hardware to interface with wiegand output readers.

### Key Features
* **Hardware Compatibility:** Fully compatible with existing Paxtogeddon hardware setups.
* **HID Support:** Decodes a wide variety of HID data formats.
* **Seamless Port:** Provides the same user experience and utility as Paxtogeddon, but for readers that provide wiegand output.

---

## Getting Started

### Hardware Requirements
To use this firmware, you will need a Paxtogeddon OLED hardware board:
[https://github.com/en4rab/Paxtogeddon-OLED-PCB](https://github.com/en4rab/Paxtogeddon-OLED-PCB)

### Installation
1.  Clone this repository:
    ```bash
    git clone https://github.com/en4rab/Hidogeddon.git
    ```
2.  Open the project in your preferred IDE (e.g., VS Code with PlatformIO or Arduino IDE).
3.  Fight with dependencys
4.  Flash the firmware to your device.

---
