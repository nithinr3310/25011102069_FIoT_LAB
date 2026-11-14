	/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bme280.h"
#include "ESPLib.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
// Assumes I2C1 is configured via CubeMX
struct bme280_dev dev;
// extern I2C_HandleTypeDef hi2c1;
uint8_t dev_addr = BME280_I2C_ADDR_PRIM;
struct bme280_settings settings;
struct bme280_data sensor_data;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */
int8_t BME280_Config(void); 
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int8_t user_i2c_read(uint8_t reg_addr,
                     uint8_t *reg_data,
                     uint32_t len,
                     void *intf_ptr)
{
    uint8_t sensor_address = *(uint8_t *)intf_ptr;
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Read(&hi2c1,
                              sensor_address << 1,
                              reg_addr,
                              I2C_MEMADD_SIZE_8BIT,
                              reg_data,
                              len,
                              HAL_MAX_DELAY);
        if (status == HAL_OK)
    {
        return BME280_OK;
    }
    else
    {
        return BME280_E_COMM_FAIL;
    }
}
int8_t user_i2c_write(uint8_t reg_addr,
                      const uint8_t *reg_data,
                      uint32_t len,
                      void *intf_ptr)
{
    uint8_t sensor_address = *(uint8_t *)intf_ptr;

    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Write(&hi2c1,
                               sensor_address << 1,
                               reg_addr,
                               I2C_MEMADD_SIZE_8BIT,
                               (uint8_t *)reg_data,
                               len,
                               HAL_MAX_DELAY);

    if (status == HAL_OK)
    {
        return BME280_OK;
    }
    else
    {
        return BME280_E_COMM_FAIL;
    }
}
void user_delay_us(uint32_t period, void *intf_ptr)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = period * (HAL_RCC_GetHCLKFreq() / 1000000U);

    while ((DWT->CYCCNT - start) < cycles);
}
int8_t BME280_Config(void)
{
    int8_t rslt;
    uint8_t settings_sel;

    // Configure Bosch interface
    dev.intf = BME280_I2C_INTF;
    dev.intf_ptr = &dev_addr;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    // Initialize sensor
    rslt = bme280_init(&dev);

    if (rslt != BME280_OK)
        return rslt;

    // Oversampling
    settings.osr_t = BME280_OVERSAMPLING_2X;
    settings.osr_p = BME280_OVERSAMPLING_16X;
    settings.osr_h = BME280_OVERSAMPLING_1X;

    // Filtering and standby
    settings.filter = BME280_FILTER_COEFF_16;
    settings.standby_time = BME280_STANDBY_TIME_1000_MS;

    // Select settings
    settings_sel = BME280_SEL_OSR_TEMP |
                   BME280_SEL_OSR_PRESS |
                   BME280_SEL_OSR_HUM |
                   BME280_SEL_FILTER |
                   BME280_SEL_STANDBY;

    // Apply settings
rslt = bme280_set_sensor_settings(settings_sel, &settings, &dev);

if (rslt != BME280_OK)
{
    return rslt;
}

rslt = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &dev);

if (rslt != BME280_OK)
{
    return rslt;
}
return BME280_OK;
}
int8_t BME280_Read(void)
{
    int8_t rslt;

    rslt = bme280_get_sensor_data(
        BME280_ALL,
        &sensor_data,
        &dev
    );

    return rslt;
}
void BME280_CreateJSON(char *payload, size_t size)
{
    snprintf(payload, size,
             "{\"temperature\":%.2f,"
             "\"pressure\":%.2f,"
             "\"humidity\":%.2f}",
             sensor_data.temperature,
             sensor_data.pressure / 100.0,
             sensor_data.humidity);
}
void BME280_Publish(void)
{
    char payload[128];

    BME280_CreateJSON(payload, sizeof(payload));

    MQTT_publishTopic("snu/iot/bme280", payload, QoS0);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);

int8_t rslt = BME280_Config();

if (rslt == BME280_OK)
{
    // BME280 initialization successful
}
else
{
    Error_Handler();
}
  // 1. Initialize the Wi-Fi module on USART1
char rx[64];

// --- ESP debug: runs first, forever, until you see OK ---
while (1)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)"AT\r\n", 4, 100);

    uint16_t n = 0;
    while (n < sizeof(rx) - 1 &&
           HAL_UART_Receive(&huart1, (uint8_t*)&rx[n], 1, 300) == HAL_OK)
        n++;
    rx[n] = '\0';

    // print to ST-Link virtual COM (enable USART2 in CubeMX)
    // HAL_UART_Transmit(&huart2, (uint8_t*)rx, n, 100);

    if (strstr(rx, "OK")) break;   // ESP alive, continue to real init
    HAL_Delay(1000);
}

ESP8266_Init(StationMode, huart1);
  // 2. Connect to your local Wi-Fi network (Replace with your Router SSID and Password)
  ESP8266_wifiConnect("SNUC", "snu12345");
  // 3. Connect to your MQTT Broker (Replace with your Broker's IP Address)
  // 1883 is the standard unencrypted MQTT port
  ESP8266_portConnect("TCP", "broker.emqx.io", "1883");
  MQTT_InitTypeDef_t mqttSetting = {0};
  mqttSetting.keepAlive = 500;
  mqttSetting.connectFlag = CleanSession;
  mqttSetting.clientID = "STM32_Publisher";
  // 5. Handshake with the MQTT broker
  MQTT_connectBroker(&mqttSetting);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /* USER CODE BEGIN WHILE */

  while (1)
  {
        if (BME280_Read() == BME280_OK)
          {
              BME280_Publish();
          }

    HAL_Delay(1000);

  }

  /* USER CODE END WHILE */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
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
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
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
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
