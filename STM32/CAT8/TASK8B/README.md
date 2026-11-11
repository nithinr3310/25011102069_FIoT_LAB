# CAT8 – Task 8C: Short Press vs Long Press

## EXP
CAT8

## DATE
07/11/2026

## NAME OF EXP
Short Press and Long Press Based LED Control Using STM32

## HARDWARE SETUP / PLATFORM
STM32 Blue Pill – STM32F103C8T6

- PA0 – Push-button input
- PC13 – Onboard LED output

## TASK NO
Task 8C

## Circuit Diagram
![Circuit Diagram](circuit.png)

## Core Logic / Pseudocode / Code Base
1. Read the push-button input.
2. Apply software debouncing with a 30 ms delay.
3. Measure the duration of each button press.
4. For a short press (less than 1 second), toggle the LED.
5. For a long press (1 second or more), toggle blinking mode.
6. In blinking mode, toggle the LED every 500 ms.

Source code: `Core/Src/main.c`

## INFERENCE
Implemented short-press and long-press detection using STM32 GPIO and software debouncing. The LED responds differently depending on the duration of the button press.