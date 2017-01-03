/*********************************************************************************************************
*
* File                : 24C02.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "camera/SCCB.h"
#include "I2C.h"

/*******************************************************************************
* Function Name  : SCCB_GPIO_Config
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void SCCB_GPIO_Init(void)
{
	I2C_Init_(Open407V_SCCB);
}

/**
  * @brief  Writes a byte at a specific Camera register
  * @param  Device: OV2640 write address.
  * @param  Addr: OV2640 register address. 
  * @param  Data: data to be written to the specific register 
  * @retval 0x00 if write operation is OK. 
  *         0xFF if timeout condition occured (device not connected or bus error).
  */
uint8_t DCMI_SingleRandomWrite(uint8_t Reg, uint8_t Data)
{
	if (I2C_Write(Open407V_SCCB, &Reg, 1, OV2640_DEVICE_WRITE_ADDRESS) == Error) return 0xFF;
	if (I2C_Write(Open407V_SCCB, &Data, 1, OV2640_DEVICE_WRITE_ADDRESS) == Error) return 0xFF;
	return 0;
}

/**
  * @brief  Reads a byte from a specific Camera register 
  * @param  Device: OV2640 write address.
  * @param  Addr: OV2640 register address. 
  * @retval data read from the specific register or 0xFF if timeout condition 
  *         occured. 
  */
uint8_t DCMI_SingleRandomRead(uint8_t Reg, uint8_t *Data)
{
	if (I2C_Write(Open407V_SCCB, &Reg, 1, OV2640_DEVICE_READ_ADDRESS) == Error) return 0xFF;
	if (I2C_Read(Open407V_SCCB, Data, 1, OV2640_DEVICE_READ_ADDRESS) == Error) return 0xFF;
	return 0;
}
