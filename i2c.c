
#include "I2C.h"

#define Timed(x) Timeout = I2C_FLAG_TIMEOUT_; while (x) { if (Timeout-- == 0) goto errReturn;}

/* 
 *  See AN2824 STM32F10xxx I2C optimized examples
 *
 *  This code implements polling based solution
 *
 */

/**
 *  Names of events used in stdperipheral library
 *
 *      I2C_EVENT_MASTER_MODE_SELECT                          : EV5
 *      I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED            : EV6     
 *      I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED               : EV6
 *      I2C_EVENT_MASTER_BYTE_RECEIVED                        : EV7
 *      I2C_EVENT_MASTER_BYTE_TRANSMITTING                    : EV8
 *      I2C_EVENT_MASTER_BYTE_TRANSMITTED                     : EV8_2
 *     
 **/



/*
 *  Read process is documented in RM008
 *
 *   There are three cases  -- read  1 byte  AN2824 Figure 2 
 *                             read  2 bytes AN2824 Figure 2
 *                             read >2 bytes AN2824 Figure 1
 */  

Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t *buf, uint32_t nbyte, uint8_t SlaveAddress)
{
  __IO uint32_t Timeout = 0;

  //    I2Cx->CR2 |= I2C_IT_ERR;  interrupts for errors 	

  if (!nbyte)
    return Success;

  // Wait for idle I2C interface
  Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	
  // Enable Acknowledgement, clear POS flag
  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  //I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

  // Intiate Start Sequence (wait for EV5
  I2C_GenerateSTART(I2Cx, ENABLE);
  Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));	

  // Send Address
  I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);

  // EV6 
  Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR));
  //Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	if (nbyte == 1) {

		// Clear Ack bit      
		I2C_AcknowledgeConfig(I2Cx, DISABLE);       

		// EV6_1 -- must be atomic -- Clear ADDR, generate STOP
		(void) I2Cx->SR2;                           
		//Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));		
		I2C_GenerateSTOP(I2Cx,ENABLE);      

		// Receive data   EV7
		Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));

		*buf++ = I2C_ReceiveData(I2Cx);

	}
	else if (nbyte == 2) {

     // EV6_1 -- must be atomic and in this order
     (void) I2Cx->SR2;                           // Clear ADDR flag
     I2C_AcknowledgeConfig(I2Cx, DISABLE);       // Clear Ack bit

     // EV7_3  -- Wait for BTF, program stop, read data twice
     Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));

     I2C_GenerateSTOP(I2Cx,ENABLE);
     *buf++ = I2Cx->DR;

     *buf++ = I2Cx->DR;

	}
  else {
		
		
		(void) I2Cx->SR2;                           // Clear ADDR flag
		while (nbyte-- != 3)
		{
			// EV7 -- cannot guarantee 1 transfer completion time, wait for BTF 
			//        instead of RXNE
			Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF)); 
			*buf++ = I2C_ReceiveData(I2Cx);
		}

		Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));  

		// EV7_2 -- Figure 1 has an error, doesn't read N-2 !
		I2C_AcknowledgeConfig(I2Cx, DISABLE);           // clear ack bit

		*buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-2
		I2C_GenerateSTOP(I2Cx,ENABLE);                  // program stop
 
		*buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-1

		// wait for byte N
		Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)); 
		*buf++ = I2C_ReceiveData(I2Cx);

		nbyte = 0;

	}

  // Wait for stop
  Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
	I2C2->SR1 |= (uint16_t)0x0400;
  
	return Success;

errReturn:

  // Any cleanup here
  return Error;

}

/*
 * Read buffer of bytes -- AN2824 Figure 3
 */

Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf,  uint32_t nbyte, uint8_t SlaveAddress)
{
	__IO uint32_t Timeout = 0;
	__IO uint32_t timeout = 0;

	/* Enable Error IT (used in all modes: DMA, Polling and Interrupts */
	//    I2Cx->CR2 |= I2C_IT_ERR;

	if (nbyte){		
		Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
		
		// Intiate Start Sequence
		I2C_GenerateSTART(I2Cx, ENABLE);
		Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));	

		// Send Address  EV5
		I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
		Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

		// EV6
		// Write first byte EV8_1	
		I2C_SendData(I2Cx, *buf++);

		while (--nbyte) {

			// wait on BTF
			Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));  
			I2C_SendData(I2Cx, *buf++);
		}

		Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF)); 
		
		//I2Cx->SR1 |= (uint16_t)0x0400;		
		I2C_GenerateSTOP(I2Cx, ENABLE);
		Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
		
	}
	
	return Success;
errReturn:
	return Error;
}


void I2C_Init_(I2C_TypeDef* I2Cx)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;

	// Enable GPIOB clocks
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


	// Configure I2C clock and GPIO 
	GPIO_StructInit(&GPIO_InitStructure);
	if (I2Cx == I2C1){

		/* GPIO clock enable */
		RCC_AHB1PeriphClockCmd(I2C1_GPIO_CLK, ENABLE);			

		/* I2C1 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

		/* I2C1 SDA and SCL configuration */
		GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN | I2C1_SDA_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_Init(I2C1_GPIO_PORT, &GPIO_InitStructure);

		GPIO_PinAFConfig(I2C1_GPIO_PORT, I2C1_SDA_SOURCE, GPIO_AF_I2C1);
		GPIO_PinAFConfig(I2C1_GPIO_PORT, I2C1_SCL_SOURCE, GPIO_AF_I2C1);

		/* I2C1 Reset */
		//RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
		//RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

	} else {

		/* GPIO clock enable */
		RCC_AHB1PeriphClockCmd(I2C2_GPIO_CLK, ENABLE);		

		/* I2C2 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

		/* I2C1 SDA and SCL configuration */
		GPIO_InitStructure.GPIO_Pin = I2C2_SCL_PIN | I2C2_SDA_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_Init(I2C2_GPIO_PORT, &GPIO_InitStructure);

		GPIO_PinAFConfig(I2C2_GPIO_PORT, I2C2_SCL_SOURCE, GPIO_AF_I2C2);
		GPIO_PinAFConfig(I2C2_GPIO_PORT, I2C2_SDA_SOURCE, GPIO_AF_I2C2);

		/* I2C2  Reset */
		//RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
		//RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);

	}
	
	I2C_DeInit(I2Cx);
	/* Configure I2Cx                */
	I2C_StructInit(&I2C_InitStructure);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_CLK_SPEED;
	
	I2C_Init(I2Cx, &I2C_InitStructure);
	I2C_Cmd(I2Cx, ENABLE);
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

}

void I2C_DeInit_(I2C_TypeDef* I2Cx)
{

	if (I2Cx == I2C1){
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	} else {
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
	}	
	
	I2C_DeInit(I2Cx);	
	I2C_Cmd(I2Cx, DISABLE);
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	
}

