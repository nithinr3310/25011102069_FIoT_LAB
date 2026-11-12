\# Experiment: Hardware Timer and Interrupt-Based Timing for LED Fading, Pulse Generation, and Stepper Motor Control



\## Objective

To design and implement a non-blocking, timer-driven embedded control system using the AT89S52 microcontroller where hardware timer interrupts serve as the system time base without using blocking software delay loops\[cite: 8, 9].



\## Sub-Task Organization

\* \*\*`TASK1\_Timer\_Base/`\*\* - Timer 0 configuration for a precise 1 ms time base and 1-second interval LED blinking\[cite: 8, 9].

\* \*\*`TASK2\_PWM\_SquareWave/`\*\* - Software PWM implementation for smooth LED fading (0% to 100% and back) alongside variable-frequency square-wave generation\[cite: 9].



\## Hardware Specifications

\* \*\*Microcontroller:\*\* AT89S52 (11.0592 MHz Crystal)\[cite: 8, 9]

\* \*\*Timer Configuration:\*\* Timer 0, Mode 1 (16-bit timer) with pre-load value `0xFC66` for 1 ms ticks\[cite: 9].

\* \*\*Outputs:\*\* 

&#x20; \* `P1.0` - 1-Second Periodic Timer Toggle\[cite: 9]

&#x20; \* `P1.1` - Software PWM LED Fading Output\[cite: 9]

&#x20; \* `P1.2` - Variable-Frequency Square-Wave Output\[cite: 9]

