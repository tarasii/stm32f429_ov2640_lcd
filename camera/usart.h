#ifndef _USART_H
#define _USART_H

#include <stdio.h>
#include "stm32f4xx.h"
#include "config.h"

void USART_Configuration(void);
void USART_NVIC_Config(void);
void USART3_Transmit(uint8_t ch_data);
#endif /*_USART_H*/
