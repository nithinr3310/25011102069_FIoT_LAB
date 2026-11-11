# CAT8 – TASK 8B: Event-Based Control

## EXP
CAT8

## DATE
07/11/2026

## NAME OF EXP
STM32 GPIO Bring-Up and Switch-Controlled LED

## HARDWARE SETUP / PLATFORM
STM32 Blue Pill – STM32F103C8T6
- PA0 – Push-button input
- PC13 – Onboard LED output

## TASK NO
Task 8B

## Circuit Diagram
![Circuit Diagram](circuit.png)

## Core Logic / Pseudocode / Code Base
1. Read the push-button input continuously.
2. Detect changes in the input state.
3. Wait for 30 ms of stable input to debounce the switch.
4. Detect a valid button press.
5. Toggle the onboard LED on each valid press.

Source code: `Src/main.c`

## INFERENCE
Implemented event-based GPIO control with software switch debouncing. The project compiled successfully in Keil MDK-ARM.