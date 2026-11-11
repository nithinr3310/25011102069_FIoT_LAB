\# Task 6A: External Interrupt-Based Control



\## 📋 Objective

To develop an Embedded C application for the AT89S52 microcontroller utilizing external hardware interrupts (`EX0` / `EX1`) to handle asynchronous events and execute real-time ISR-driven responses.



\## 🛠️ Hardware \& Pin Configuration

\* \*\*Microcontroller:\*\* AT89S52 (11.0592 MHz Crystal)\[cite: 13]

\* \*\*Interrupt Pins:\*\* `INT0` (Pin P3.2) and `INT1` (Pin P3.3) connected to push buttons.

\* \*\*Output Peripherals:\*\* Status LEDs connected to Port 1 (`P1.0 - P1.7`).



\## 🚀 Implementation Details

\* Configures interrupt control registers (`IE` and `TCON`) for edge or level-triggered interrupts.

\* Implements a clean Interrupt Service Routine (ISR) to toggle peripheral states upon external button presses.

\* Minimizes latency inside the ISR to maintain predictable system behavior.



\## 📷 Circuit Schematic

!\[Circuit Diagram](circuit.png)

