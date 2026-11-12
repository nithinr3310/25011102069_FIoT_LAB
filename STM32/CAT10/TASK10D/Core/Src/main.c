/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : STM32 + ESP8266 (AT firmware) tiny web server
  ******************************************************************************
  * Flow: join Wi-Fi -> start TCP server on port 80 -> wait for GET request
  *       -> reply with an HTML page -> close the connection.
  *
  * Wiring: USART6 <-> ESP8266 (STM TX -> ESP RX, STM RX -> ESP TX, common GND)
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
#define WIFI_SSID   "SNUC"
#define WIFI_PASS   "snu12345"
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);

/* USER CODE BEGIN PFP */
static void ESP_Send(const char *s);
void ESP_HandleBrowserRequest(int);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

static void ESP_Send(const char *s)
{
  HAL_UART_Transmit(&huart6, (uint8_t *)s, strlen(s), 1000);
}

void ESP_HandleBrowserRequest(int value)
{
  char rx[600];
  char cmd[40];
  char resp[256];
  char budy[] = "<html><body><h1>STM32 IoT Node</h1><p>Hello from STM32</p></body></html>";

  memset(rx, 0, sizeof(rx));

  __HAL_UART_CLEAR_OREFLAG(&huart6);
  HAL_UART_Receive(&huart6, (uint8_t *)rx, sizeof(rx) - 1, 500);


  char *p = strstr(rx, "+IPD,");
  if (p == NULL || strstr(rx, "GET /") == NULL)
  {
    return;
  }
  char id = p[5];

  int body_len = strlen(budy);
  int len = snprintf(resp, sizeof(resp),
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: %d\r\n"
                     "Connection: close\r\n\r\n"
                     "%s",
                     (int)body_len, budy);

  snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%c,%d\r\n", id, len);
  ESP_Send(cmd);
  HAL_Delay(300);
  HAL_UART_Transmit(&huart6, (uint8_t *)resp, len, 1000);
  HAL_Delay(300);

  snprintf(cmd, sizeof(cmd), "AT+CIPCLOSE=%c\r\n", id);
  ESP_Send(cmd);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();

  /* USER CODE BEGIN 2 */

  /* ---- ESP8266 setup ---- */
  ESP_Send("AT\r\n");
  HAL_Delay(1000);

  ESP_Send("AT+CWMODE=1\r\n");
  HAL_Delay(1000);

  char join[96];
  snprintf(join, sizeof(join), "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASS);
  ESP_Send(join);
  HAL_Delay(10000);

  ESP_Send("AT+CIFSR\r\n");
  HAL_Delay(1000);

  ESP_Send("AT+CIPMUX=1\r\n");
  HAL_Delay(1000);

  ESP_Send("AT+CIPSERVER=1,80\r\n");
  HAL_Delay(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    ESP_HandleBrowserRequest(5);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  */
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

/**
  * @brief USART6 Initialization Function (connected to ESP8266)
  */
static void MX_USART6_UART_Init(void)
{
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
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
