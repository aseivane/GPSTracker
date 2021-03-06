/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GPS/GPSmodel.h"
#include "GPS/GPScontroller.h"
#include "oled/Screens.h"

//#include "fatfs_sd.h"
//#include "SDlogging.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UART1 &huart1
#define I2C_ADDRESS 0x3C
#define LONG_TIME_PRESS 3000

#define SIG_BUTTON 1U
#define SEL_BUTTON 2U
//#undef DEBUG
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
/*struct Stat {
    uint8_t powerSave : 1;
    uint8_t number : 3;
    uint8_t time_updated : 1;
	uint8_t date_updated : 1;
	uint8_t GPSupdated : 1;
	uint8_t button_pressed : 1;
}Status;*/


volatile uint8_t usart_rx_dma_buffer[MAX_NMEA_LEN];
uint8_t copy_buffer[MAX_NMEA_LEN];
volatile uint32_t timePress;

uint8_t recording = 0;
uint8_t new_data = 0;
uint16_t new_data_size = 0;
uint8_t screen_number;
uint8_t GPSupdated = 0;
uint8_t screen_power = 0;
uint8_t button_pressed=0;
uint8_t refresh_screen =1;


//u8x8_t u8x8;                    // u8x8 object
u8g2_t u8g2;
GPSdata gps;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void start_recording( )
{
#ifdef DEBUG
	HAL_UART_Transmit(&huart3, (uint8_t *)"***recording***\r\n",
						strlen("***recording***\r\n"),1000);
#endif
	recording = 1;
	/* when it's recording only wakes up for
	 * saving data
	 */
	screen_power = 1;
	u8g2_SetPowerSave(&u8g2, screen_power);
	HAL_TIM_Base_Stop_IT(&htim3);
	//HAL_PWR_EnableSleepOnExit ();
	//HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void stop_recording( )
{
#ifdef DEBUG
	HAL_UART_Transmit(&huart3, (uint8_t *)"***notrecording***\r\n",
						strlen("***notrecording***\r\n"),1000);
#endif
	recording = 0;
	/* when it's recording only wakes up for
	 * saving data
	 */
	screen_power = 0;
	u8g2_SetPowerSave(&u8g2, screen_power);
	HAL_TIM_Base_Start_IT(&htim3);
}

/* TIM IT handler*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
#ifdef DEBUG
	HAL_UART_Transmit(&huart3, (uint8_t *)"***ENTER TIM***\r\n",
						strlen("***ENTER TIM***\r\n"),1000);
#endif

	refresh_screen = 1;
#ifdef DEBUG
	HAL_UART_Transmit(&huart3, (uint8_t *)"***EXIT TIM***\r\n\r\n",
						strlen("***EXIT TIM***\r\n\r\n"),1000);
#endif
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART1)
	{

#ifdef DEBUG
	HAL_UART_Transmit(&huart3, (uint8_t *)"***ENTER IT_RXIDLE***\r\n",
						strlen("***ENTER IT_RXIDLE***\r\n"),1000);
	//HAL_UART_Transmit(&huart3, usart_rx_dma_buffer, Size,1000);
#endif
	new_data = 1;
	new_data_size = Size;
	memcpy(copy_buffer, usart_rx_dma_buffer, new_data_size);

#ifdef DEBUG
	HAL_UART_Transmit(&huart3, (uint8_t *)"***EXIT IT_RXIDLE***\r\n\r\n",
						strlen("***EXIT IT_RXIDLE***\r\n\r\n"),1000);
#endif

		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, usart_rx_dma_buffer, MAX_NMEA_LEN);	//Configure DMA
	  	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
#ifdef DEBUG
	HAL_UART_Transmit(&huart3, (uint8_t *)"***ENTER EXTI***\r\n",
						strlen("***ENTER EXTI***\r\n"),1000);
#endif

	/* waits until chack_buttons functions clears the event */
	if ( button_pressed && !recording) return;

	/* if the button is pressed, stores ti ticks at that time */
	if (HAL_GPIO_ReadPin(GPIOA,GPIO_Pin) == GPIO_PIN_RESET )
	{
		timePress = HAL_GetTick();
		return;
	}

	/* when the button is release, meassures de time difference */
	timePress = HAL_GetTick() - timePress;

	switch(GPIO_Pin)
	{
		case SIG_Pin:
			button_pressed = SIG_BUTTON;
			break;

		case SEL_Pin:
			button_pressed = SEL_BUTTON;
			HAL_PWR_DisableSleepOnExit ();
			break;
	}
#ifdef DEBUG
	HAL_UART_Transmit(&huart3, (uint8_t *)"***EXIT EXTI***\r\n\r\n",
						strlen("***EXIT EXTI***\r\n\r\n"),1000);
#endif
}

void check_buttons()
{
	switch(button_pressed)
	{
		case SIG_BUTTON:
			if( timePress < LONG_TIME_PRESS )
				screen_number == 1 ? screen_number = 0 : screen_number++;
			else
			{
				screen_power ^= 1 << 1;
				u8g2_SetPowerSave(&u8g2, screen_power);
			}
			break;

		case SEL_BUTTON:
			if(recording)
			{
				stop_recording();
			}
			else
			{
				start_recording();
			}
			break;
	}
	button_pressed = 0;
}

/**
  * @brief  Init and setup Sreen, GPS, SD.
  * @retval -
  */
void Setup()
{
	initScreen(&u8g2); // Screen init, clear, home screen set

	initGPS(&gps);	// Initialize default values for gps object

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  MX_FATFS_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /*Setup GPS model, OLED and SD*/
  Setup();

  /*Manual peripherial config*/
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, usart_rx_dma_buffer, MAX_NMEA_LEN);	//Configure DMA
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  HAL_TIM_Base_Start_IT(&htim3);	//Configure TIM3 for OLED update

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //HAL_SuspendTick();
  //HAL_PWR_EnableSleepOnExit ();
  //HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  while (1)
  {
	  if (new_data)
	  {
		  new_data = 0;
#ifdef DEBUG
		  HAL_UART_Transmit(&huart3, (uint8_t *)"***new_data***\r\n\r\n",
							strlen("***new_data***\r\n\r\n"),1000);
#endif

		  /* copies DMA buffer in to local buffer */

		  copy_buffer[new_data_size] = '\0'; // adds an EOS

		  /* updates data */
#ifdef DEBUG
		  HAL_UART_Transmit(&huart3, (uint8_t *)"***updateGPS***\r\n\r\n",
							strlen("***updateGPS***\r\n\r\n"),1000);
#endif
		  updateGPS(&gps, copy_buffer, &new_data_size);
		  GPSupdated = 1;
#ifdef DEBUG
		  HAL_UART_Transmit(&huart3, (uint8_t *)"***updateDateTime***\r\n\r\n",
							strlen("***updateDateTime***\r\n\r\n"),1000);
#endif
		  updateDateTime( &hrtc, copy_buffer, &new_data_size);
	  }

	  if ( recording )
	  {
		  /* Is there new position info?*/
		  if( GPSupdated )
		  {
#ifdef DEBUG
			  HAL_UART_Transmit(&huart3, (uint8_t *)"***log_data***\r\n\r\n",
			  							strlen("***log_data***\r\n\r\n"),1000);
#endif
			  //HAL_NVIC_DisableIRQ(DMA1_Channel5_IRQn);
			  log_data(&gps, &hrtc);	// Saves data in SD
			  GPSupdated = 0;
			  //HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

			  //HAL_UART_Transmit(&huart3, (uint8_t *)"***__enable_irq***\r\n\r\n",
			  //			  							strlen("***__enable_irq***\r\n\r\n"),1000);
		  }
		  //HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	  }

	  if (refresh_screen && !screen_power)
	  {
		  refresh_screen =0;
		  __disable_irq();
#ifdef DEBUG
		  HAL_UART_Transmit(&huart3, (uint8_t *)"***refresh_screen***\r\n\r\n",
								strlen("***refresh_screen***\r\n\r\n"),1000);
#endif
		  updateScreen();	// Screen update
		  __enable_irq();
	  }


	  check_buttons();
	  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  hi2c1.Init.ClockSpeed = 400000;
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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_FRIDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 1;
  DateToUpdate.Year = 21;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
  /*
	RTC_AlarmTypeDef Alarm;
	Alarm.Alarm = RTC_ALARM_A;
	HAL_RTC_GetTime(&hrtc, &(Alarm.AlarmTime), RTC_FORMAT_BIN);
	Alarm.AlarmTime.Minutes += 1;
	HAL_RTC_SetAlarm_IT (&hrtc, &Alarm, RTC_FORMAT_BIN);
  __HAL_RTC_ALARM_EXTI_ENABLE_IT();
  __HAL_RTC_ALARM_ENABLE_IT(&hrtc,RTC_IT_ALRA);*/
  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 10000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  huart1.Init.BaudRate = 9600;
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SIG_Pin SEL_Pin */
  GPIO_InitStruct.Pin = SIG_Pin|SEL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

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

#ifdef  USE_FULL_ASSERT
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
