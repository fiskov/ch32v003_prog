/*
 * peripheral.h
 *
 *  Created on: May 2, 2024
 */

#ifndef USER_PERIPHERAL_H_
#define USER_PERIPHERAL_H_

#include "stdint.h"

uint16_t GetMCUFlashSize(void);
uint32_t GetMCUUID1(void);
uint32_t GetMCUUID2(void);
uint32_t GetMCUUID3(void);

uint32_t GetCHIPID(void);

void led_on(void);
void led_off(void);
void led_toggle(void);
void led_init(void);

void timer_led_init(uint16_t led_blink_period_ms);

void USARTx_CFG(void);

void pin_all_pulldown_init(void);
void SysTick_init_1ms(void);
uint32_t SysTick_get_count(void);

void sleep_start(uint32_t sleep_period_s);

void uart_send_string(char *bfr);

#endif /* USER_PERIPHERAL_H_ */
