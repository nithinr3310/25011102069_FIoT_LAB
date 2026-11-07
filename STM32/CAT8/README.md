# CAT8 — STM32 GPIO Bring-Up and Switch-Controlled LED

## EXP
CAT8

## DATE
18/09/2026 (Week 7)

## NAME OF EXP
STM32 GPIO Bring-Up and Switch-Controlled LED

## HARDWARE SETUP / PLATFORM
STM32 development board — the assessment permits Blue Pill or Nucleo.
**Exact board model and GPIO pin assignments: pending confirmation.**

## TASK NO
- Task 8A — GPIO Input/Output
- Task 8B — Event-Based Control
- Task 8C — Short Press vs Long Press

## Circuit Diagram
[circuit.png](circuit.png)

> Temporary placeholder only. Replace with the verified circuit after confirming the exact board, button pin, LED pin, polarity, and wiring.

## Core Logic / Pseudocode / Code Base

### Task 8A — GPIO Input/Output
Read the push-button state and make the LED follow the switch state.

### Task 8B — Event-Based Control
Detect a valid button press and toggle the LED once per press. Account for switch bounce with a software debounce strategy.

### Task 8C — Short Press vs Long Press
Measure the press duration using a non-blocking time-base approach:
- Short press: less than 1 second — toggle LED.
- Long press: at least 1 second — enter continuous blinking mode.
- Next short press — stop blinking.
- Long press again — return to blinking mode.

**Assessment constraint:** Do not use a blocking delay to determine whether the button is being held for a long press.

## INFERENCE
To be completed after implementing and testing Tasks 8A–8C on the selected STM32 board.

## Project status
- [x] Repository folder scaffold created
- [ ] Confirm exact STM32 board and pin assignments
- [ ] Generate CubeMX project and Keil MDK files
- [ ] Add application source code
- [ ] Add verified circuit diagram
- [ ] Build and verify in Keil
- [ ] Record observations and inference
