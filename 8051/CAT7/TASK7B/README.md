\# Task 7B: Cursor Positioning and Dynamic Value Update



\## 📋 Objective

To extend the 4-bit LCD driver with explicit cursor navigation and dynamic value-rendering functions (`lcd\_goto`, `lcd\_puts`, and `lcd\_print\_number`) for real-time data display on the AT89S52\[cite: 13].



\## 🛠️ Hardware \& Pin Configuration

\* \*\*Microcontroller:\*\* AT89S52 (11.0592 MHz Crystal)\[cite: 13]

\* \*\*Display:\*\* 16×2 Character LCD (HD44780 Driver, 4-bit mode)\[cite: 13, 14]

\* \*\*Pin Mappings:\*\* Same as Task 7A (`RS` on `P2.0`, `EN` on `P2.1`, `D4-D7` on `P2.4-P2.7`)\[cite: 13, 14, 15].



\## 🚀 Implementation Details

\* \*\*`lcd\_goto(row, column)`\*\*: Computes target DDRAM addresses (`0x80` for row 1, `0xC0` for row 2) combined with column offsets.

\* \*\*`lcd\_print\_number(value)`\*\*: Converts integer counter variables into ASCII character streams for flicker-free real-time updates\[cite: 13].



\## 📷 Circuit Schematic

!\[Circuit Diagram](circuit.png)

