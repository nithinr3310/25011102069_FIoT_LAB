# CAT13 – BME280 Sensor Interfacing Using I²C and MQTT

## EXP
CAT13

## DATE
28/10/2026

## NAME OF EXP
BME280 Sensor Interfacing Using I²C and MQTT using Custom Built ESP8266 API

## HARDWARE SETUP / PLATFORM
- STM32 NUCLEO-F401RE
- BME280 Temperature, Pressure and Humidity Sensor
- ESP8266 Wi-Fi Module
- Keil µVision IDE
- ARMCLANG Compiler V6.24
- STM32 HAL Drivers
- Node-RED
- MQTT Protocol

## TASK NO
CAT13

## OBJECTIVE
To interface the BME280 environmental sensor with the STM32 microcontroller using the I²C communication protocol and transmit the sensor readings to an MQTT broker through an ESP8266 Wi-Fi module using a custom-built ESP8266 API.

## CIRCUIT DIAGRAM
See `circuit.png` in this directory.

## HARDWARE CONNECTIONS

| Component | STM32 Pin | Function |
|---|---|---|
| BME280 SDA | PB6 | I²C1 SDA |
| BME280 SCL | PB7 | I²C1 SCL |
| ESP8266 TX | PA3 | USART2 RX |
| ESP8266 RX | PA2 | USART2 TX |
| BME280 VCC | 3.3V | Power |
| ESP8266 VCC | 3.3V | Power |
| Common GND | GND | Ground |

## SOFTWARE IMPLEMENTATION

1. Initialize the I²C peripheral to communicate with the BME280 sensor.
2. Configure the BME280 using the Bosch sensor API.
3. Read compensated temperature, pressure and humidity measurements.
4. Initialize the ESP8266 using the custom `ESPLib.c` and `ESPLib.h` library.
5. Configure the ESP8266 in Station Mode and connect it to a Wi-Fi network.
6. Establish an MQTT connection with the broker.
7. Convert the sensor measurements into a JSON payload.
8. Publish the sensor data periodically to the configured MQTT topic.

## MQTT CONFIGURATION

| Parameter | Configuration |
|---|---|
| Broker | broker.emqx.io |
| Port | 1883 |
| Protocol | MQTT |
| Client ID | STM32_BME280 |
| Topic | stm32/test/data |
| QoS | 0 |
| Keep Alive | 500 seconds |
| Clean Session | Enabled |

### Sample JSON Payload

```json
{
  "status": "success",
  "temperature": 28.45,
  "humidity": 62.30,
  "pressure": 1008.72
}
```

## NODE-RED IMPLEMENTATION

- Configure an MQTT input node to subscribe to `stm32/test/data`.
- Use a JSON node to parse the incoming MQTT payload.
- Display temperature, humidity and pressure using dashboard widgets.
- Monitor the incoming sensor measurements through the Node-RED dashboard.

## INFERENCE

The experiment implements I²C-based BME280 sensor interfacing with an STM32 microcontroller and MQTT-based wireless data transmission using an ESP8266 module. The custom ESP8266 API provides Wi-Fi and MQTT communication functionality, while Node-RED can be used to visualize the published environmental measurements.

## BUILD AND VERIFICATION

- IDE: Keil µVision
- Compiler: ARMCLANG V6.24
- Build output: 0 errors and 0 warnings (reported build; application functionality requires hardware verification).

**Note:** Physical sensor readings, Wi-Fi connectivity and MQTT message delivery were not hardware-tested.

## REPOSITORY STRUCTURE

```text
CAT13/
│
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── bme280.h
│   │   ├── bme280_defs.h
│   │   └── ESPLib.h
│   │
│   └── Src/
│       ├── main.c
│       ├── bme280.c
│       ├── ESPLib.c
│       ├── stm32f4xx_it.c
│       └── stm32f4xx_hal_msp.c
│
├── Drivers/
├── circuit.png
├── README.md
├── cat13-mqtt.ioc
└── cat13-mqtt.uvprojx
```
