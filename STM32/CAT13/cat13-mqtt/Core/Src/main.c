
/* Includes */
#include "main.h"
#include "ESPLib.h"
#include "bme280.h"
#include <stdio.h>
#include <stdint.h>

/* Peripheral handles */
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;	

/* BME280 variables */
struct bme280_dev dev;
struct bme280_data comp_data;
struct bme280_settings settings;

static uint8_t dev_addr = BME280_I2C_ADDR_PRIM;

int8_t rslt;
float temperature, pressure, humidity;
char mqtt_payload[100];

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);

int8_t user_i2c_read(uint8_t reg_addr,
                     uint8_t *reg_data,
                     uint32_t len,
                     void *intf_ptr);

int8_t user_i2c_write(uint8_t reg_addr,
                      const uint8_t *reg_data,
                      uint32_t len,
                      void *intf_ptr);

void user_delay_us(uint32_t period, void *intf_ptr);


/* BME280 I2C read callback */
int8_t user_i2c_read(uint8_t reg_addr,
                     uint8_t *reg_data,
                     uint32_t len,
                     void *intf_ptr)
{
    uint8_t address = *(uint8_t *)intf_ptr;

    if (HAL_I2C_Mem_Read(&hi2c1,
                         address << 1,
                         reg_addr,
                         I2C_MEMADD_SIZE_8BIT,
                         reg_data,
                         len,
                         HAL_MAX_DELAY) == HAL_OK)
    {
        return BME280_OK;
    }

    return BME280_E_COMM_FAIL;
}


/* BME280 I2C write callback */
int8_t user_i2c_write(uint8_t reg_addr,
                      const uint8_t *reg_data,
                      uint32_t len,
                      void *intf_ptr)
{
    uint8_t address = *(uint8_t *)intf_ptr;

    if (HAL_I2C_Mem_Write(&hi2c1,
                          address << 1,
                          reg_addr,
                          I2C_MEMADD_SIZE_8BIT,
                          (uint8_t *)reg_data,
                          len,
                          HAL_MAX_DELAY) == HAL_OK)
    {
        return BME280_OK;
    }

    return BME280_E_COMM_FAIL;
}


/* BME280 delay callback */
void user_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;

    HAL_Delay((period / 1000U) + 1U);
}


int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART2_UART_Init();

    /* ---------------- BME280 INITIALIZATION ---------------- */

    dev.intf_ptr = &dev_addr;
    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    rslt = bme280_init(&dev);

    if (rslt != BME280_OK)
    {
        Error_Handler();
    }

    /* Configure oversampling and filter */
    settings.osr_h = BME280_OVERSAMPLING_1X;
    settings.osr_p = BME280_OVERSAMPLING_1X;
    settings.osr_t = BME280_OVERSAMPLING_1X;
    settings.filter = BME280_FILTER_COEFF_OFF;

    rslt = bme280_set_sensor_settings(
        BME280_SEL_OSR_PRESS |
        BME280_SEL_OSR_TEMP  |
        BME280_SEL_OSR_HUM   |
        BME280_SEL_FILTER,
        &settings,
        &dev
    );

    if (rslt != BME280_OK)
    {
        Error_Handler();
    }

    /* Enable continuous measurements */
    rslt = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &dev);

    if (rslt != BME280_OK)
    {
        Error_Handler();
    }

    /* ---------------- ESP8266 / MQTT INITIALIZATION ---------------- */

    /* Initialize ESP8266 using USART2 */
    ESP8266_Init(StationMode, huart2);

    /* Connect ESP8266 to Wi-Fi */
    ESP8266_wifiConnect("YOUR_WIFI_NAME",
                        "YOUR_WIFI_PASSWORD");

    /* Connect to the MQTT broker */
    ESP8266_portConnect("TCP",
                        "broker.emqx.io",
                        "1883");

    /* Configure MQTT client */
    MQTT_InitTypeDef_t mqttSetting;

    mqttSetting.keepAlive = 500;
    mqttSetting.connectFlag = CleanSession;
    mqttSetting.clientID = "STM32_BME280";

    /* Establish the MQTT session */
    MQTT_connectBroker(&mqttSetting);


    /* ---------------- MAIN LOOP ---------------- */

    while (1)
    {
        /* Read temperature, pressure and humidity */
        rslt = bme280_get_sensor_data(
            BME280_ALL,
            &comp_data,
            &dev
        );

        if (rslt == BME280_OK)
        {
            /*
             * Bosch default integer mode:
             * temperature is in 0.01 degree C
             * pressure is in Q24.8 Pa
             * humidity is in Q22.10 %RH
             */

            temperature = comp_data.temperature / 100.0f;

            pressure = (comp_data.pressure / 256.0f) / 100.0f;
            /* Pressure converted to hPa */

            humidity = comp_data.humidity / 1024.0f;

            /* Format the sensor values as an MQTT payload */
            snprintf(mqtt_payload,
                     sizeof(mqtt_payload),
                     "{\"temperature\":%.2f,"
                     "\"pressure\":%.2f,"
                     "\"humidity\":%.2f}",
                     (double)temperature,
                     (double)pressure,
                     (double)humidity);

            /* Publish the sensor data */
            MQTT_publishTopic(
                "stm32/nithin/data",
                mqtt_payload,
                QoS0
            );
        }

        HAL_Delay(1000);
    }
}


/* ---------------- SYSTEM CLOCK ---------------- */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(
        PWR_REGULATOR_VOLTAGE_SCALE2
    );

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 84;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                            FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}


/* ---------------- I2C1 INITIALIZATION ---------------- */

static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}


/* ---------------- USART2 INITIALIZATION ---------------- */

static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}


/* ---------------- GPIO INITIALIZATION ---------------- */

static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
}


/* ---------------- ERROR HANDLER ---------------- */

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}
