/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SCCB_H
#define __SCCB_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "config.h"
					  
/* Private define ------------------------------------------------------------*/
  
#define OV2640_DEVICE_WRITE_ADDRESS    0x60
#define OV2640_DEVICE_READ_ADDRESS     0x61


/* Private function prototypes -----------------------------------------------*/
void SCCB_GPIO_Init(void);
uint8_t DCMI_SingleRandomWrite(uint8_t Addr, uint8_t Data);
uint8_t DCMI_SingleRandomRead(uint8_t Addr, uint8_t *Data);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
