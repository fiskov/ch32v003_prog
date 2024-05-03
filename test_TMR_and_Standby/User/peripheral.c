/*
 * peripheral.cpp
 *
 *  Created on: May 2, 2024
 */

#include "peripheral.h"
#include <ch32v00x.h>

uint16_t GetMCUFlashSize(void)
{
    return (*(uint16_t *)0x1FFFF7E0);
}

uint32_t GetMCUUID1(void)
{
    return (*(uint32_t *)0x1FFFF7E8);
}
uint32_t GetMCUUID2(void)
{
    return (*(uint32_t *)0x1FFFF7EC);
}

uint32_t GetMCUUID3(void)
{
    return (*(uint32_t *)0x1FFFF7F0);
}

/*********************************************************************
 * @fn      DBGMCU_GetCHIPID
 *
 * @brief   Returns the CHIP identifier.
 *
 * @return Device identifier.
 *          ChipID List-
 *    CH32V003F4P6-0x003005x0
 *    CH32V003F4U6-0x003105x0
 *    CH32V003A4M6-0x003205x0
 *    J4M6-0x003305x0
 */
uint32_t GetCHIPID(void)
{
    return (*(uint32_t *)0x1FFFF7C4);
}

#define LED_PIN GPIO_Pin_0
#define LED_PIN_PORT GPIOD
#define LED_PIN_RCC RCC_APB2Periph_GPIOD

void led_off(void)
{
    GPIO_SetBits(LED_PIN_PORT, LED_PIN);
}

void led_on(void)
{
    GPIO_ResetBits(LED_PIN_PORT, LED_PIN);
}

void led_toggle(void)
{
    if (GPIO_ReadOutputData(LED_PIN_PORT) & LED_PIN)
        led_on();
    else
        led_off();
}

void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(LED_PIN_RCC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PIN_PORT, &GPIO_InitStructure);
}

void timer_led_init(uint16_t led_blink_period_ms)
{
    TIM_TimeBaseInitTypeDef TIMBase_InitStruct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    uint16_t psc, arr;
    psc = SystemCoreClock / 1000 - 1;
    arr = led_blink_period_ms - 1;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIMBase_InitStruct.TIM_Period = arr;
    TIMBase_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIMBase_InitStruct.TIM_Prescaler = psc;
    TIMBase_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIMBase_InitStruct);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    TIM_Cmd(TIM2, ENABLE);
}

/*********************************************************************
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART peripheral.
 *
 * @return  none
 */
void USARTx_CFG(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

/**
 * Configure SysTick for increase `SysTick_GetTick()` every 1ms
 */
uint32_t _SYSTICK_GLOBAL_TIMER_1MS = 0;

void SysTick_init_1ms(void)
{
    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTick->SR &= ~(1 << 0);
    SysTick->CMP = (SystemCoreClock / 1000) - 1;
    SysTick->CNT = 0;
    SysTick->CTLR = 0xF;
}

uint32_t SysTick_get_count(void)
{
    return _SYSTICK_GLOBAL_TIMER_1MS;
}

void pin_all_pulldown_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void sleep_start(uint32_t sleep_period_s)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_LSICmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
        ;

    // Initializes EXTI collection
    EXTI_InitTypeDef EXTI_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    EXTI_DeInit();
    EXTI_InitStructure.EXTI_Line = EXTI_Line9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Init. PWR_AWU
    PWR_AWU_SetPrescaler(PWR_AWU_Prescaler_4096); // 128kHz / 4096 = 32 ms

    // correct to 1.0s
    sleep_period_s = (sleep_period_s * 1009) / 1000;

    for (int i = 0; i < sleep_period_s; i++)
    {
        PWR_AWU_SetWindowValue(30); // (30+1) * 32ms == 992ms
        PWR_AutoWakeUpCmd(ENABLE);
        PWR_EnterSTANDBYMode(PWR_STANDBYEntry_WFE);
    }
}

// UART transmite in block-mode
void uart_send_string(char *bfr)
{
    while (*bfr)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
            ;
        USART_SendData(USART1, *bfr++);
    }
}
