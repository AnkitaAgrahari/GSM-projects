/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include <string.h>
#include <stdio.h>


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define CTRL_Z 26
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t response,connection,TCP_buffer,
         SIM_APN[15],compare_data[60];
uint8_t store[80];
uint8_t state=0, new_state=1;
uint8_t send_flag=0;
uint8_t i=0;
//char APN[15]="airtelgprs.com";

char buffer[15];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

	HAL_UART_Receive_IT(&huart3, &response, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  switch(new_state)
	  {
	   case 1:
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT\r\n", 4, 1000);
	   HAL_Delay(1000);
	   break;
	   case 2:
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CPIN?\r\n", 10, 1000);
	   HAL_Delay(1000);
	   break;
	   case 3:
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CREG?\r\n", 10, 1000);
	   HAL_Delay(1000);
	   break;
	   case 4:
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CIPMUX=0\r\n", 13, 1000);
	   HAL_Delay(1000);
	   break;
	   case 5:
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CGATT=1\r\n", 12, 1000);
	   HAL_Delay(2000);
	   break;
	   case 6:
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CSTT=\"airtelgprs.com\","",""\r\n",32, 1000);
	   HAL_Delay(1000);
	   break;
	   case 7:
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CIICR\r\n", 10, 1000);
	   HAL_Delay(1000);
	   break;
	   case 8:
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CIFSR\r\n", 10, 1000);
	   HAL_Delay(1000);
	   break;
	   case 9:
//	   sprintf (connection,"AT+CIPSTART=\"TCP\",\"143.244.133.98\",\"8000\"\r\n",TCP_buffer);
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CIPSTART=\"TCP\",\"143.244.133.98\",\"8000\"\r\n",43, 1000);
	   HAL_Delay(1000);
	   break;
	   case 10:
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CIPSEND\r\n", 12, 1000);
	   HAL_Delay(1000);
	   break;
	   case 11:
	   sprintf(buffer,"TEVATRON%c\r\n",CTRL_Z);
	   HAL_UART_Transmit(&huart3, (uint8_t*)buffer, strlen(buffer),1000);
	   HAL_Delay(1000);
	   break;
	   case 12:
	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CIPCLOSE\r\n", 13,1000);
	   HAL_Delay(1000);
	   break;
//	   case 13:
//	   HAL_UART_Transmit(&huart3, (uint8_t*)"AT+CIPSHUT\r\n", 12,1000);
//	   HAL_Delay(1000);
//	   break;

	  }
//	  memset(store, '\0' , 50);
  }

  if(send_flag)
  {
	 if(strstr((char*)store,"OK")&& (new_state=1))
	  {
 		        new_state=2;
	  }
	 if(strstr((char*)store,"READY")&& (new_state=2))
	  {
	 	        new_state=3;
	  }
	 if(strstr((char*)store,"+CREG: 0,5")&& (new_state=3))
	  {
	 	 	    new_state=4;
	  }
	 if(strstr((char*)store,"CIPMUX=0")&& (new_state=4))
	  {
	 	 	    new_state=5;
	  }
	 if(strstr((char*)store,"CGATT=1")&& (new_state=5))
	  {
	 	 	 	new_state=6;
	  }
	 if(strstr((char*)store,"OK")&& (new_state=6))
	  {
	 	 	 	new_state=7;
	  }
	 if(strstr((char*)store,"AT+CIICR")&& (new_state=7))
	  {
	 	 	 	new_state=8;
	  }
	 if(strstr((char*)store,"AT+CIFSR")&& (new_state=8))
	  {
	 	 	 	new_state=9;
	  }
	 if(strstr((char*)store,"CONNECT")&& (new_state=9))
	  {
		 	 	 new_state=10;
	  }
	 if(strstr((char*)store,"AT+CIPSEND")&& (new_state=10))
	  {
			 	new_state=11;
	  }
	 if(strstr((char*)store,"SEND OK")&& (new_state=11))
	  {
	 			 new_state=12;
	  }
	 if(strstr((char*)store,"CLOSED OK")&& (new_state=12))
	  {
		 			 new_state=9;

	  }
//	 if(strstr((char*)store,"ERROR"))
//	 {
//		 new_state=13;
//	 }
//	 if(strstr((char*)store,"SHUT OK")&& (new_state=13))
//	 {
//		 new_state=1;
//		 memset(store, '0' ,50);
//	 }


//	 cmp((char*)store,"AT+CPIN?\r\n\r\n+CPIN: READY\r\n\r\nOK\r\n")==0
  }
  /* USER CODE END 3 */
}
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
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
  * @brief TIM2 Initialization Function for 1sec
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 50;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 313725-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB14 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

{
	if(huart->Instance==USART3)
	{
	store[i]=response;
	i++;
    response=0;
	HAL_UART_Receive_IT(&huart3, &response, 1);
	HAL_TIM_Base_Start_IT(&htim2);
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	i=0;
	HAL_TIM_Base_Stop_IT(&htim2);
//    send_flag=1;
    HAL_UART_Receive_IT(&huart3,&response, 1);
    send_flag=1;



}
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
