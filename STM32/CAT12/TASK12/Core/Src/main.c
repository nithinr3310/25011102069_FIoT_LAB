
#include "main.h"
#include "bme280.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* =========================================================
   PERIPHERAL HANDLES
   ========================================================= */

I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1;

/* =========================================================
   BME280 VARIABLES
   ========================================================= */

struct bme280_dev bme;
struct bme280_settings bme_settings;

/* BME280 I2C address */
uint8_t bme_i2c_address = BME280_I2C_ADDR_PRIM;

/* UART buffer */
char uart_buffer[150];

/* =========================================================
   FUNCTION PROTOTYPES
   ========================================================= */

void SystemClock_Config(void);

static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);

static int8_t user_i2c_read(uint8_t reg_addr,
                            uint8_t *reg_data,
                            uint32_t length,
                            void *intf_ptr);

static int8_t user_i2c_write(uint8_t reg_addr,
                             const uint8_t *reg_data,
                             uint32_t length,
                             void *intf_ptr);

static void user_delay_us(uint32_t period, void *intf_ptr);

static void BME280_Init(void);
static void BME280_ReadAndTransmit(void);

/* Error_Handler is already declared in main.h */
void Error_Handler(void);


/* =========================================================
   BME280 I2C READ CALLBACK
   ========================================================= */

static int8_t user_i2c_read(uint8_t reg_addr,
                            uint8_t *reg_data,
                            uint32_t length,
                            void *intf_ptr)
{
    uint8_t dev_addr = *(uint8_t *)intf_ptr;

    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Read(
                 &hi2c1,
                 (uint16_t)(dev_addr << 1),
                 reg_addr,
                 I2C_MEMADD_SIZE_8BIT,
                 reg_data,
                 (uint16_t)length,
                 HAL_MAX_DELAY
             );

    if (status == HAL_OK)
    {
        return BME280_OK;
    }

    return BME280_E_COMM_FAIL;
}


/* =========================================================
   BME280 I2C WRITE CALLBACK
   ========================================================= */

static int8_t user_i2c_write(uint8_t reg_addr,
                             const uint8_t *reg_data,
                             uint32_t length,
                             void *intf_ptr)
{
    uint8_t dev_addr = *(uint8_t *)intf_ptr;

    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Write(
                 &hi2c1,
                 (uint16_t)(dev_addr << 1),
                 reg_addr,
                 I2C_MEMADD_SIZE_8BIT,
                 (uint8_t *)reg_data,
                 (uint16_t)length,
                 HAL_MAX_DELAY
             );

    if (status == HAL_OK)
    {
        return BME280_OK;
    }

    return BME280_E_COMM_FAIL;
}


/* =========================================================
   BME280 DELAY CALLBACK
   ========================================================= */

static void user_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;

    /* Convert microseconds to milliseconds */
    HAL_Delay((period + 999U) / 1000U);
}


/* =========================================================
   BME280 INITIALIZATION
   ========================================================= */

static void BME280_Init(void)
{
    int8_t rslt;

    /* Configure BME280 I2C interface */

    bme.intf = BME280_I2C_INTF;

    /* Pass I2C address through interface pointer */
    bme.intf_ptr = &bme_i2c_address;

    bme.read = user_i2c_read;
    bme.write = user_i2c_write;
    bme.delay_us = user_delay_us;


    /* Try primary I2C address: 0x76 */

    bme_i2c_address = BME280_I2C_ADDR_PRIM;

    rslt = bme280_init(&bme);


    /* If not detected, try secondary address: 0x77 */

    if (rslt != BME280_OK)
    {
        bme_i2c_address = BME280_I2C_ADDR_SEC;

        rslt = bme280_init(&bme);
    }


    /* Check sensor initialization */

    if (rslt != BME280_OK)
    {
        const char error_msg[] =
            "ERROR: BME280 initialization failed!\r\n";

        HAL_UART_Transmit(
            &huart1,
            (uint8_t *)error_msg,
            sizeof(error_msg) - 1U,
            HAL_MAX_DELAY
        );

        Error_Handler();
    }


    /* Configure oversampling */

    bme_settings.osr_t = BME280_OVERSAMPLING_1X;
    bme_settings.osr_p = BME280_OVERSAMPLING_1X;
    bme_settings.osr_h = BME280_OVERSAMPLING_1X;

    /* Disable IIR filter */

    bme_settings.filter = BME280_FILTER_COEFF_OFF;


    /* Apply sensor settings */

    rslt = bme280_set_sensor_settings(
                BME280_SEL_OSR_TEMP |
                BME280_SEL_OSR_PRESS |
                BME280_SEL_OSR_HUM |
                BME280_SEL_FILTER,
                &bme_settings,
                &bme
           );

    if (rslt != BME280_OK)
    {
        Error_Handler();
    }


    /* Set sensor to normal mode */

    rslt = bme280_set_sensor_mode(
               BME280_POWERMODE_NORMAL,
               &bme
           );

    if (rslt != BME280_OK)
    {
        Error_Handler();
    }

    HAL_Delay(100);
}


/* =========================================================
   READ BME280 DATA AND SEND THROUGH UART
   ========================================================= */

static void BME280_ReadAndTransmit(void)
{
    struct bme280_data sensor_data;

    int8_t rslt;

    int32_t temperature_x100;

    uint32_t pressure_pa;
    uint32_t humidity_x100;


    /* Read temperature, pressure and humidity */

    rslt = bme280_get_sensor_data(
               BME280_ALL,
               &sensor_data,
               &bme
           );


    if (rslt == BME280_OK)
    {
        /*
         * Bosch integer compensation output:
         *
         * Temperature: 0.01 degree Celsius
         * Pressure:    Q24.8 Pa
         * Humidity:    Q22.10 %RH
         */

        temperature_x100 = sensor_data.temperature;

        pressure_pa = sensor_data.pressure / 256U;

        humidity_x100 =
            (sensor_data.humidity * 100U) / 1024U;


        /* Format sensor readings */

        snprintf(
            uart_buffer,
            sizeof(uart_buffer),

            "Temperature: %ld.%02ld C | "
            "Pressure: %lu Pa | "
            "Humidity: %lu.%02lu %%RH\r\n",

            (long)(temperature_x100 / 100),

            (long)(
                temperature_x100 < 0
                ? -(temperature_x100 % 100)
                : temperature_x100 % 100
            ),

            (unsigned long)pressure_pa,

            (unsigned long)(humidity_x100 / 100U),

            (unsigned long)(humidity_x100 % 100U)
        );


        /* Transmit through USART1 */

        HAL_UART_Transmit(
            &huart1,
            (uint8_t *)uart_buffer,
            strlen(uart_buffer),
            HAL_MAX_DELAY
        );
    }
    else
    {
        const char error_msg[] =
            "ERROR: BME280 sensor reading failed!\r\n";

        HAL_UART_Transmit(
            &huart1,
            (uint8_t *)error_msg,
            sizeof(error_msg) - 1U,
            HAL_MAX_DELAY
        );
    }
}


/* =========================================================
   MAIN FUNCTION
   ========================================================= */

int main(void)
{
    /* Initialize HAL library */

    HAL_Init();


    /* Configure system clock */

    SystemClock_Config();


    /* Initialize peripherals */

    MX_GPIO_Init();

    MX_I2C1_Init();

    MX_USART1_UART_Init();


    /* Startup message */

    const char startup_msg[] =
        "\r\nCAT12 BME280 SENSOR INTERFACING\r\n"
        "Initializing sensor...\r\n";

    HAL_UART_Transmit(
        &huart1,
        (uint8_t *)startup_msg,
        sizeof(startup_msg) - 1U,
        HAL_MAX_DELAY
    );


    /* Initialize BME280 */

    BME280_Init();


    /* Initialization success message */

    const char success_msg[] =
        "BME280 initialized successfully!\r\n\r\n";

    HAL_UART_Transmit(
        &huart1,
        (uint8_t *)success_msg,
        sizeof(success_msg) - 1U,
        HAL_MAX_DELAY
    );


    /* Infinite loop */

    while (1)
    {
        BME280_ReadAndTransmit();

        HAL_Delay(1000);
    }
}


/* =========================================================
   SYSTEM CLOCK CONFIGURATION
   HSI = 16 MHz
   SYSCLK = 84 MHz
   ========================================================= */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


    /* Enable power control clock */

    __HAL_RCC_PWR_CLK_ENABLE();


    /* Voltage scaling */

    __HAL_PWR_VOLTAGESCALING_CONFIG(
        PWR_REGULATOR_VOLTAGE_SCALE2
    );


    /* Configure oscillator */

    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState = RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_DEFAULT;


    /* PLL configuration */

    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;

    RCC_OscInitStruct.PLL.PLLSource =
        RCC_PLLSOURCE_HSI;

    RCC_OscInitStruct.PLL.PLLM = 16;

    RCC_OscInitStruct.PLL.PLLN = 336;

    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;

    RCC_OscInitStruct.PLL.PLLQ = 7;


    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }


    /* Configure clock dividers */

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;


    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_PLLCLK;

    RCC_ClkInitStruct.AHBCLKDivider =
        RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider =
        RCC_HCLK_DIV2;

    RCC_ClkInitStruct.APB2CLKDivider =
        RCC_HCLK_DIV1;


    if (HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_2
        ) != HAL_OK)
    {
        Error_Handler();
    }
}


/* =========================================================
   I2C1 INITIALIZATION
   PB6 = SCL
   PB7 = SDA
   ========================================================= */

static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;

    hi2c1.Init.ClockSpeed = 100000;

    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;

    hi2c1.Init.OwnAddress1 = 0;

    hi2c1.Init.AddressingMode =
        I2C_ADDRESSINGMODE_7BIT;

    hi2c1.Init.DualAddressMode =
        I2C_DUALADDRESS_DISABLE;

    hi2c1.Init.OwnAddress2 = 0;

    hi2c1.Init.GeneralCallMode =
        I2C_GENERALCALL_DISABLE;

    hi2c1.Init.NoStretchMode =
        I2C_NOSTRETCH_DISABLE;


    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}


/* =========================================================
   USART1 INITIALIZATION
   PA9  = TX
   PA10 = RX
   115200 baud
   ========================================================= */

static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;

    huart1.Init.BaudRate = 115200;

    huart1.Init.WordLength = UART_WORDLENGTH_8B;

    huart1.Init.StopBits = UART_STOPBITS_1;

    huart1.Init.Parity = UART_PARITY_NONE;

    huart1.Init.Mode = UART_MODE_TX_RX;

    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;

    huart1.Init.OverSampling = UART_OVERSAMPLING_16;


    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}


/* =========================================================
   GPIO INITIALIZATION
   MSP configuration is in stm32f4xx_hal_msp.c
   ========================================================= */

static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
}


/* =========================================================
   ERROR HANDLER
   ========================================================= */

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}
