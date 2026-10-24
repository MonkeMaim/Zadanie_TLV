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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tlv.h"
#include <string.h> // Potrzebne do bezpiecznego kopiowania pamięci (memcpy)
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void Process_Received_Data(const uint8_t *rx_buffer, size_t rx_len);

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    uint8_t rx_buffer[32]; // Bufor przechowujący odebraną ramkę TLV

    // KROK 1: Odbiór nagłówka ramki (2 pierwsze bajty: Type oraz Length)
    // Funkcja blokuje wykonanie do momentu odebrania nagłówka lub wystąpienia timeoutu
    if (HAL_UART_Receive(&huart2, rx_buffer, 2, HAL_MAX_DELAY) == HAL_OK)
    {
      uint8_t payload_len = rx_buffer[1]; // Odczytanie długości payloadu z drugiego bajtu

      // KROK 2: Walidacja bezpieczeństwa – sprawdzenie limitu payloadu z pliku msg.h
      if (payload_len <= TLV_MAX_PAYLOAD_SIZE)
      {
        // KROK 3: Odbiór właściwych danych (payloadu), jeśli ich długość jest większa od zera
        if (payload_len > 0)
        {
          // Dopisanie reszty bajtów bezpośrednio za nagłówkiem (od indeksu 2)
          HAL_UART_Receive(&huart2, &rx_buffer[2], payload_len, HAL_MAX_DELAY);
        }

        // KROK 4: Skompletowanie pełnej ramki i przekazanie jej do przetwarzania
        size_t total_len = 2 + payload_len; // Całkowity rozmiar ramki (nagłówek + payload)
        Process_Received_Data(rx_buffer, total_len);
      }
      else
      {
        // Opcjonalne zabezpieczenie: odrzucenie ramki z nieobsługiwaną długością
        // (Można tutaj dodać flush bufora UART, jeśli zajdzie taka potrzeba)
      }
    }

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

  /** Configure the main internal regulator output voltage
   */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
 * @brief Przetwarza odebrany strumień bajtów zgodnie z protokołem TLV,
 *        rozpoznaje typ komunikatu i wykonuje odpowiednią akcję.
 *
 * @param[in] rx_buffer Wskaźnik na bufor z surowymi bajtami odebranymi przez UART.
 * @param[in] rx_len    Liczba odebranych bajtów w buforze.
 */
void Process_Received_Data(const uint8_t *rx_buffer, size_t rx_len)
{
  packet_t rx_packet; // Struktura przechowująca zdekodowaną ramkę

  // Dekodowanie i wstępna walidacja ramki TLV
  if (TLV_Decode(rx_buffer, rx_len, &rx_packet) != TLV_OK)
  {
    return; // Odrzucenie uszkodzonej lub niepełnej ramki
  }

  // Obsługa komunikatów w zależności od ich identyfikatora (Type)
  switch (rx_packet.message_id)
  {
  case MSG_PING:
  {
    msg_ping_t ping_data;

    // Bezpieczne skopiowanie payloadu do struktury żądania PING
    memcpy(&ping_data, rx_packet.payload, sizeof(msg_ping_t));

    // Opóźnienie systemowe zgodne z parametrem odebranym w ramce
    HAL_Delay(ping_data.pong_delay_ms);

    // Przygotowanie komunikatu odpowiedzi PONG
    msg_pong_t pong_data;
    pong_data.reserved = 0;

    packet_t tx_packet;
    tx_packet.message_id = MSG_PONG;
    tx_packet.payload_length = sizeof(msg_pong_t);

    // Skopiowanie danych PONG do payloadu ramki wyjściowej
    memcpy(tx_packet.payload, &pong_data, sizeof(msg_pong_t));

    // Enkodowanie ramki do surowego bufora bajtów
    uint8_t tx_buffer[32];
    size_t tx_len = 0;

    if (TLV_Encode(&tx_packet, tx_buffer, sizeof(tx_buffer), &tx_len) == TLV_OK)
    {
      // Transmisja odpowiedzi przez interfejs UART
      extern UART_HandleTypeDef huart2;
      HAL_UART_Transmit(&huart2, tx_buffer, tx_len, HAL_MAX_DELAY);
    }
    break;
  }

  case MSG_MATH:
  {
    msg_math_t math_req;
    memcpy(&math_req, rx_packet.payload, sizeof(msg_math_t));

    msg_math_resp_t math_resp;
    math_resp.flags = 0;
    math_resp.result = 0;

    // Zmienna pomocnicza do bezpiecznego wykrywania przepełnień zakresu
    int32_t temp_result = 0;

    // Wybór operacji arytmetycznej na podstawie pól bitowych
    if (math_req.operation_bits.add)
    {
      temp_result = (int32_t)math_req.operand_a + math_req.operand_b;
    }
    else if (math_req.operation_bits.sub)
    {
      temp_result = (int32_t)math_req.operand_a - math_req.operand_b;
    }
    else if (math_req.operation_bits.mul)
    {
      temp_result = (int32_t)math_req.operand_a * math_req.operand_b;
    }
    else if (math_req.operation_bits.div)
    {
      if (math_req.operand_b == 0)
      {
        math_resp.flags_bits.divide_by_zero = 1; // Ustawienie flagi błędu
      }
      else
      {
        temp_result = (int32_t)math_req.operand_a / math_req.operand_b;
      }
    }

    // Walidacja zakresu wyniku dla zmiennych 16-bitowych ze znakiem
    if (temp_result > 32767 || temp_result < -32768)
    {
      math_resp.flags_bits.overflow = 1; // Ustawienie flagi przepełnienia
    }
    else
    {
      if (math_resp.flags_bits.divide_by_zero == 0)
      {
        math_resp.result = (int16_t)temp_result;
      }
    }

    // Przygotowanie ramki odpowiedzi z wynikami obliczeń
    packet_t tx_packet;
    tx_packet.message_id = MSG_MATH_RESP;
    tx_packet.payload_length = sizeof(msg_math_resp_t);

    memcpy(tx_packet.payload, &math_resp, sizeof(msg_math_resp_t));

    // Enkodowanie i wysyłka zwrotna przez UART
    uint8_t tx_buffer[32];
    size_t tx_len = 0;

    if (TLV_Encode(&tx_packet, tx_buffer, sizeof(tx_buffer), &tx_len) == TLV_OK)
    {
      extern UART_HandleTypeDef huart2;
      HAL_UART_Transmit(&huart2, tx_buffer, tx_len, HAL_MAX_DELAY);
    }
    break;
  }

  default:
    // Ignorowanie nierozpoznanych typów komunikatów
    break;
  }
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
