/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Multiprocessor communication mode routine:
 *Master:USART1_Tx(PD5)\USART1_Rx(PD6).
 *This routine demonstrates that USART1 receives the data sent by CH341 and inverts
 *it and sends it (baud rate 115200).
 *
 *Hardware connection:PD5 -- Rx
 *                     PD6 -- Tx
 *
 */

#include "debug.h"
#include "peripheral.h"

#define LED_BLINK_PERIOD_MS 50
#define WORK_PERIOD_S       3
#define SLEEP_PERIOD_S      10

#define UART_SPEED          115200

/* Global define */

/* Global Variable */

static uint32_t timer_until(uint32_t *timer, uint32_t period_ms)
{
  uint32_t timer_1ms = SysTick_get_count();
  if (timer_1ms - *timer >= period_ms)
  {
    *timer = timer_1ms;
    return 1;
  }
  return 0;
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  SystemCoreClockUpdate();
  SysTick_init_1ms();

  // programmer is not connected if MCU in deepsleep (???)
  for (volatile int i = 0; i < 5E5; i++)
    __NOP();

  // reduce power consumption
  pin_all_pulldown_init();

  USART_Printf_Init(UART_SPEED);

  printf("\033[2J"); // clear screen
  printf("SystemClk:%dHz, FlashSize:%dkB\r\n", SystemCoreClock, GetMCUFlashSize());
  // printf("ChipID:%08x\r\n", GetCHIPID());
  printf("UID:%08x %08x %08x\r\n", GetMCUUID1(), GetMCUUID2(), GetMCUUID3());

  USARTx_CFG();

  led_init();
  led_on();

  timer_led_init(LED_BLINK_PERIOD_MS / 2);

  uint32_t timer_sleep = 0;
  uint32_t timer_char = 0, pos_char = 0;
  char bfr_wait[] = {'|', '/', '-', '\\'};

  printf("%6d: start\r\n", SysTick_get_count());

  while (1)
  {
    __WFI();
    if (timer_until(&timer_sleep, WORK_PERIOD_S * 1000))
    {
      printf("%6d: sleep\r\n", SysTick_get_count());
      led_off();
      sleep_start(SLEEP_PERIOD_S);
      USART_Printf_Init(UART_SPEED);
      led_on();
      printf("%6d: wake\r\n", SysTick_get_count());
    }

    if (timer_until(&timer_char, 100))
    {
      char bfr[] = {bfr_wait[pos_char++ % 4], '\r', 0};

      uart_send_string(bfr);
    }
  }
}

void timer_led_trigger(void) // call it from timer interrupt
{
  led_toggle();
}
