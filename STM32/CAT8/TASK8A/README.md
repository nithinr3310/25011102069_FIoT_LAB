# TASK 8A — GPIO Input/Output

## EXP
CAT8

## DATE
18/09/2026

## NAME OF EXP
STM32 GPIO Bring-Up and Switch-Controlled LED

## HARDWARE SETUP / PLATFORM
- STM32 Blue Pill (STM32F103C8T6)
- Onboard LED: PC13
- Push-button input: PA0
- System clock: 72 MHz

## TASK NO
Task 8A — GPIO Input/Output

## Circuit Diagram
![Circuit Diagram](circuit.png)

## Core Logic / Pseudocode / Code Base

1. Configure PA0 as GPIO input with pull-up.
2. Configure PC13 as GPIO output.
3. Continuously read the push-button input.
4. When PA0 is LOW, set PC13 LOW to turn ON the onboard LED.
5. When PA0 is HIGH, set PC13 HIGH to turn OFF the onboard LED.

### Code Base
See `Src/main.c`.

## INFERENCE
The STM32 GPIO input and output functionality was implemented. The push-button input controls the onboard LED through GPIO polling. The project compiled successfully in Keil MDK-ARM.