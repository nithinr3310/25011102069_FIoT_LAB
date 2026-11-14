# Foundations of IoT Lab

Firmware implementations, circuit diagrams, and documentation for the **Foundations of Internet of Things (IoT)** laboratory assessments across **8051** and **STM32** platforms.

---

## 📌 Experiments Overview

### Platform: 8051 (AT89S52)

| Assessment | Directory | Topic / Focus | Target MCU |
|---|---|---|---|
| **CAT 2** | [`8051/CAT2/TASK2B`](8051/CAT2/TASK2B/README.md) | Basic LED Blinking & GPIO Bring-Up | AT89S52 |
| **CAT 3** | [`8051/CAT3/TASK3A`](8051/CAT3/TASK3A/README.md) | Pushbutton-Controlled Multi-Mode LED Sequencing | AT89S52 |
| **CAT 4** | [`8051/CAT4/TASK4A`](8051/CAT4/TASK4A/README.md) | 7-Segment Display & Matrix Multiplexing | AT89S52 |
| **CAT 5** | [`8051/CAT5`](8051/CAT5/README.md) | Hardware Timer 0, Software PWM Fading & Stepper Sequence | AT89S52 |
| **CAT 6** | [`8051/CAT6`](8051/CAT6/README.md) | UART Serial Initialization & Character Echo (9600 Baud) | AT89S52 |
| **CAT 7** | [`8051/CAT7`](8051/CAT7/README.md) | 4×4 Matrix Keypad Scanning & 16×2 LCD Interfacing | AT89S52 |

---

### Platform: STM32 (Cortex-M)

| Assessment | Directory | Topic / Focus | Target MCU |
|---|---|---|---|
| **CAT 8** | [`STM32/CAT8`](STM32/CAT8/README.md) | GPIO Input/Output, Debouncing & Short/Long Press Detection | STM32F103C8Tx |
| **CAT 9** | [`STM32/CAT9`](STM32/CAT9/TASK9A/README.md) | USART Serial Communication & Command String Parser | STM32F103C8Tx |
| **CAT 10** | [`STM32/CAT10`](STM32/CAT10/README.md) | STM32 to ESP8266 (Wemos) UART Bridge & AT Command Interface | STM32F103C8Tx |
| **CAT 11** | [`STM32/CAT11`](STM32/CAT11/README.md) | Wi-Fi Station Mode & Embedded TCP / HTTP Web Server | STM32F103C8Tx / STM32F410RBTx |
| **CAT 12** | [`STM32/CAT12`](STM32/CAT12/README.md) | Nucleo On-Board LED Bring-Up, Button Debouncing & Short/Long Press Detection | STM32F401RE |
| **CAT 13** | [`STM32/CAT13/TASK13`](STM32/CAT13/TASK13/README.md) | BME280 Sensor Interfacing via I²C with Bosch Sensor API | STM32F401RE |
| **CAT 14** | [`STM32/CAT14/TASK14`](STM32/CAT14/TASK14/README.md) | IoT Cloud Node: BME280 Sensor + ESP8266 Wi-Fi + MQTT to Node-RED | STM32F401RE |
| **CAT 15** | [`STM32/CAT15`](STM32/CAT15/README.md) | Environmental IoT Node & MQTT Telemetry (Personal Final Project) | STM32F401RE |

---

## 📂 Repository Structure

```text
Foundations_of_IoT/
├── 8051/
│   ├── CAT2/TASK2B/       # Basic LED Blinking & GPIO Bring-Up
│   ├── CAT3/TASK3A/       # Multi-Mode LED Sequencing & Push-Button Control
│   ├── CAT4/TASK4A/       # 7-Segment Display & Matrix Keypad Multiplexing
│   ├── CAT5/              # Hardware Timer 0 & Waveforms
│   │   ├── TASK5A/        # Software PWM LED Fading
│   │   ├── TASK5B/        # Square-Wave Generation (5B.1 Freq, 5B.2 Duty, 5B.3 Pulse)
│   │   └── TASK5C/        # Stepper Motor Periodic Sequence Control
│   ├── CAT6/              # UART Serial Communication (9600 Baud)
│   │   ├── TASK6A/        # Periodic String Transmission
│   │   └── TASK6B/        # Character Echo over Terminal
│   └── CAT7/              # 16x2 LCD Driver & 4x4 Matrix Keypad
│       ├── TASK7A/        # 4-bit Mode LCD Initialization & String Display
│       ├── TASK7B/        # Cursor Positioning & Real-Time Counter Display
│       └── TASK7C/        # Interactive Keypad-Driven Character Movement
│
├── STM32/
│   ├── CAT8/              # GPIO Input/Output & Debouncing (STM32F103C8Tx)
│   │   ├── TASK8A/        # Continuous GPIO Read & LED Drive
│   │   ├── TASK8B/        # Debounced Event-Based Toggle
│   │   └── TASK8C/        # Short-Press vs Long-Press Blinking Control
│   ├── CAT9/              # USART Serial Communication (STM32F103C8Tx)
│   │   ├── TASK9A/        # Full-Duplex Character Reception & Terminal Echo
│   │   └── TASK9B/        # Command String Parser (LED ON, LED OFF, LED TOGGLE)
│   ├── CAT10/             # ESP8266 Wi-Fi Modem AT Commands (STM32F103C8Tx)
│   │   ├── TASK10A/       # STM32 to ESP8266 UART Bridge (AT Ping)
│   │   └── TASK10B/       # AT Interface Command Sequencing & Response Relay
│   ├── CAT11/             # ESP8266 Network & Server (STM32F103C8 / STM32F410RB)
│   │   ├── TASK11A/       # Embedded TCP HTTP Web Server (Port 80)
│   │   └── TASK11B/       # Wi-Fi Station Join & IP Configuration
│   ├── CAT12/             # Nucleo GPIO & User Input (STM32F401RE)
│   │   ├── TASK12A/       # On-Board LED (LD2) Bring-Up
│   │   ├── TASK12B/       # On-Board Switch (B1) Debouncing & State Retention
│   │   └── TASK12C/       # Short-Press vs Long-Press Blink Mode Switching
│   ├── CAT13/TASK13/      # BME280 Environmental Sensor Interfacing (I²C + Bosch API)
│   ├── CAT14/TASK14/      # IoT Cloud Telemetry (BME280 + ESP8266 + MQTT to Node-RED)
│   └── CAT15/TASK15/      # End-to-End Environmental IoT Node (Personal Final Project)
│
└── README.md
```

Each task folder contains:
- `README.md` — Experiment objectives, hardware connections, logic explanation, and inference.
- `circuit.png` — Hardware schematic / circuit diagram.
- Firmware source code, Keil/CubeMX project configuration files, and compiled binaries.

---

## 🛠 Tools & Technologies

- **8051 Platform:** Keil µVision C51, AT89S52 Microcontroller
- **STM32 Platform:** Keil MDK-ARM, STM32CubeMX, STM32 HAL Drivers, STM32F103C8Tx (Blue Pill), STM32F401RE (Nucleo), STM32F410RBTx
- **Peripherals & Sensors:** BME280 (Temperature, Humidity, Pressure), ESP8266 (Wi-Fi Modem), 16×2 LCD (HD44780), 4×4 Matrix Keypad, 7-Segment Display, Stepper Motor
- **Protocols & Cloud:** I²C, UART, MQTT, HTTP, Node-RED

---

## 👤 Author

- **Nithin R** — [GitHub Profile](https://github.com/nithinr3310)