\# Task 7C: 4x4 Keypad-Based Character Movement



\## 📋 Objective

To build an interactive embedded application integrating a 4×4 matrix keypad with the 4-bit LCD driver, allowing users to move a character dynamically across the 16×2 display using directional keys\[cite: 13].



\## 🛠️ Hardware \& Pin Configuration

\* \*\*Microcontroller:\*\* AT89S52 (11.0592 MHz Crystal)\[cite: 13]

\* \*\*Display:\*\* 16×2 Character LCD (HD44780 Driver, 4-bit mode)\[cite: 13, 14]

\* \*\*Input Device:\*\* 4×4 Matrix Keypad connected to Port 1 (`P1.0 - P1.7`)\[cite: 13]



\## 🚀 Implementation Details

\* Scans row and column lines to capture matrix key presses.

\* Maps navigation keys to coordinate shifts:

&#x20; \* \*\*Key `2`:\*\* Move Up ($row = 1$)\[cite: 13]

&#x20; \* \*\*Key `8`:\*\* Move Down ($row = 2$)\[cite: 13]

&#x20; \* \*\*Key `4`:\*\* Move Left ($col--$)\[cite: 13]

&#x20; \* \*\*Key `6`:\*\* Move Right ($col++$)\[cite: 13]



\## 📷 Circuit Schematic

!\[Circuit Diagram](circuit.png)

