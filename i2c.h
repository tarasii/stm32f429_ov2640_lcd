#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"

//+------+------+------+
//| I2C1               |
//+------+------+------+
//| SDA  | SLC  | SMBA |
//+------+------+------+
//| PB6  | PB7  | PB5  |
//| PB8  | PB9  |      |
//+------+------+------+
//| I2C2               |
//+------+------+------+
//| PB10 | PB11 | PB12 |
//+------+------+------+

#define I2C1_SDA_PIN        	GPIO_Pin_6
#define I2C1_SDA_SOURCE       GPIO_PinSource6
#define I2C1_SCL_PIN          GPIO_Pin_7
#define I2C1_SCL_SOURCE       GPIO_PinSource7

#define I2C1_GPIO_PORT        GPIOB
#define I2C1_GPIO_CLK         RCC_AHB1Periph_GPIOB


#define I2C2_SDA_PIN          GPIO_Pin_11
#define I2C2_SDA_SOURCE       GPIO_PinSource11
#define I2C2_SCL_PIN          GPIO_Pin_10
#define I2C2_SCL_SOURCE       GPIO_PinSource10

#define I2C2_GPIO_PORT        GPIOB
#define I2C2_GPIO_CLK         RCC_AHB1Periph_GPIOB


#define I2C_CLK_SPEED         10000 //100000
#define I2C_OWN_ADDRESS7      0xF
#define I2C_FLAG_TIMEOUT_   	10000

typedef enum {Error = 0, Success = !Error } Status;


Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t* buf, uint32_t nbuf, uint8_t SlaveAddress);
Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbuf,  uint8_t SlaveAddress);
void I2C_Init_(I2C_TypeDef* I2Cx);
void I2C_DeInit_(I2C_TypeDef* I2Cx);

#endif
