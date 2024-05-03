/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v00x_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main Interrupt Service Routines.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include <ch32v00x_it.h>

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));


/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
  printf("!!! NMI_Handler !!!\r\n");
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
  printf("!!! HardFault_Handler !!!\r\n");
  while (1)
  {
  }
}

// SysTick interrupt routine

extern uint32_t _SYSTICK_GLOBAL_TIMER_1MS;

__attribute__((interrupt("WCH-Interrupt-fast")))
void SysTick_Handler(void)
{
  SysTick->SR = 0;

  _SYSTICK_GLOBAL_TIMER_1MS++;
}


// Timer1 interrupt routine

extern void timer_led_trigger(void);

__attribute__((interrupt("WCH-Interrupt-fast")))
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //

        timer_led_trigger();
    }
}
