#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include "stm32f4xx.h"

/**
 * @brief Definition for COM port4, connected to USART3
 */ 
#define Open407V_USART                        USART3
#define Open407V_USART_CLK                    RCC_APB1Periph_USART3

#define Open407V_USART_TX_PIN                 GPIO_Pin_10
#define Open407V_USART_TX_GPIO_PORT           GPIOC
#define Open407V_USART_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define Open407V_USART_TX_SOURCE              GPIO_PinSource10
#define Open407V_USART_TX_AF                  GPIO_AF_USART3

#define Open407V_USART_RX_PIN                 GPIO_Pin_11
#define Open407V_USART_RX_GPIO_PORT           GPIOC
#define Open407V_USART_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define Open407V_USART_RX_SOURCE              GPIO_PinSource11
#define Open407V_USART_RX_AF                  GPIO_AF_USART3

#define Open407V_USART_IRQn                   USART3_IRQn


/**
 * @brief Definition for LCD
 */
 /* Configure LCD pins: PB1->Reset and PB0->Back Light Control */
#define Open407V_LCD_BackLightControl_PIN		GPIO_Pin_0
#define Open407V_LCD_BackLightControl_PORT		GPIOB
#define Open407V_LCD_BackLightControl_CLK		RCC_AHB1Periph_GPIOB

#define Open407V_LCD_Reset_PIN					GPIO_Pin_1
#define Open407V_LCD_Reset_PORT					GPIOB
#define Open407V_LCD_Reset_CLK					RCC_AHB1Periph_GPIOB	

/* DCMI Communication boards Interface */

/**
 * @connected to I2C2
 */
 /* Configure I2C1 pins: PB10->SIOC and PB11->SIOD */ 
#define Open407V_SCCB                        	I2C2
#define Open407V_SCCB_CLK                    	RCC_APB1Periph_I2C2

#define Open407V_SCCB_SDA_PIN                 	GPIO_Pin_11
#define Open407V_SCCB_SDA_GPIO_PORT           	GPIOB
#define Open407V_SCCB_SDA_GPIO_CLK            	RCC_AHB1Periph_GPIOB
#define Open407V_SCCB_SDA_SOURCE              	GPIO_PinSource11
#define Open407V_SCCB_SDA_AF                  	GPIO_AF_I2C2

#define Open407V_SCCB_SCL_PIN                 	GPIO_Pin_10
#define Open407V_SCCB_SCL_GPIO_PORT           	GPIOB
#define Open407V_SCCB_SCL_GPIO_CLK            	RCC_AHB1Periph_GPIOB
#define Open407V_SCCB_SCL_SOURCE              	GPIO_PinSource10
#define Open407V_SCCB_SCL_AF                  	GPIO_AF_I2C2

#endif	  /*_CONFIG_H*/

