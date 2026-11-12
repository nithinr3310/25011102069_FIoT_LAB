# Foundations of IoT Lab

Firmware implementations, circuit diagrams, and documentation for the **Foundations of Internet of Things (IoT)** laboratory assessments across **8051** and **STM32** platforms.

---

## 📌 Experiments Overview

### Platform: 8051 (AT89S52)

| Assessment | Directory | Topic / Focus |
|---|---|---|
| **CAT 2** | `8051/CAT2/TASK2B` | Basic LED Blinking & GPIO Bring-Up |
| **CAT 3** | `8051/CAT3/TASK3A` | Pushbutton-Controlled Multi-Mode LED Sequencing |
| **CAT 4** | `8051/CAT4/TASK4A` | 7-Segment Display & Matrix Multiplexing |
| **CAT 5** | `8051/CAT5/` | Hardware Timer 0 (1 ms base), Waveform Generation & Software PWM |
| **CAT 6** | `8051/CAT6/TASK6A-B` | UART Serial Initialization & Character Echo (9600 Baud) |
| **CAT 7** | `8051/CAT7/TASK7A-C` | 4×4 Matrix Keypad Scanning & 16×2 LCD Interfacing |

---

### Platform: STM32 (Cortex-M)

| Assessment | Directory | Topic / Focus | Target MCU |
|---|---|---|---|
| **CAT 8** | `STM32/CAT8/TASK8A-C` | GPIO Input/Output, Debouncing & Short/Long Press Detection | STM32F103C8Tx |
| **CAT 9** | `STM32/CAT9/TASK9A-B` | USART Serial Communication & Command String Parser | STM32F103C8Tx |
| **CAT 10** | `STM32/CAT10/TASK10A-C` | STM32 to ESP8266 (Wemos) UART Bridge, AT Commands & Wi-Fi Station Mode | STM32F103C8Tx |
| **CAT 11** | `STM32/CAT11/TASK11A-B` | Nucleo On-Board LED Bring-Up & EXTI Pushbutton Interrupts | STM32F401RE |
| **CAT 12** | `STM32/CAT12/TASK12` | BME280 Sensor Interfacing via I²C with Bosch Sensor API | STM32F401RE |
| **CAT 13** | `STM32/CAT13/TASK13` | IoT Cloud Node: BME280 Sensor + ESP8266 Wi-Fi + MQTT to Node-RED | STM32F401RE |

---

## 📂 Repository Structure

```text
Foundations_of_IoT/
├── 8051/
│   ├── CAT2/TASK2B/       # LED Blinking
│   ├── CAT3/TASK3A/       # LED Modes & Switch
│   ├── CAT4/TASK4A/       # 7-Segment Display
│   ├── CAT5/              # Timers & PWM
│   ├── CAT6/TASK6A, 6B/   # UART Serial
│   └── CAT7/TASK7A-C/     # 4x4 Keypad & 16x2 LCD
│
├── STM32/
│   ├── CAT8/TASK8A-C/     # GPIO & Button Events
│   ├── CAT9/TASK9A-B/     # USART & Command Logic
│   ├── CAT10/TASK10A-C/   # ESP8266 Wi-Fi & AT Commands
│   ├── CAT11/TASK11A-B/   # Nucleo Onboard GPIO & EXTI
│   ├── CAT12/TASK12/      # BME280 I2C Sensor
│   └── CAT13/TASK13/      # BME280 + MQTT Cloud Telemetry
│
└── README.md
```

Each task folder contains:
- `README.md` — Experiment objectives, pinouts, and logic explanation.
- `circuit.png` — Circuit connection schematic.
- Source code, project configuration files, and compiled binaries.

---

## 🛠 Tools Used

- **8051:** Keil µVision C51
- **STM32:** Keil MDK-ARM, STM32CubeMX, STM32 HAL Drivers
- **Hardware:** AT89S52, STM32F103C8Tx (Blue Pill), STM32F401RE (Nucleo), BME280, ESP8266
- **IoT & Network:** MQTT, Node-RED

---

## 👤 Author

- **Nithin R** — [GitHub Profile](https://github.com/nithinr3310)
