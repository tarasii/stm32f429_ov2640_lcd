/**
  ******************************************************************************
  * @file    DCMI/OV2640_Camera/dcmi_OV2640.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   This file includes the driver for OV2640 Camera module mounted on 
  *          STM322xG-EVAL board RevA and RevB.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "dcmi.h"


/** @addtogroup DCMI Settings
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Bits definitions ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//static void Delay(uint32_t nTime); 
//static void Delay_ms(uint32_t nTime);

/**
  * @brief  Configures the DCMI to interface with the OV2640 camera module.
  * @param  None
  * @retval None
  */
	
	
void DCMI_GPIO_Init(void)
{
	DCMI_InitTypeDef DCMI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	DCMI_CROPInitTypeDef DCMI_Crop;
	
  /* Enable DCMI GPIOs clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                         RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOI, ENABLE);


  /* Enable DCMI clock */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

  /* Connect DCMI pins to AF13 ************************************************/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);
	
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);
	
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_DCMI);
	
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_DCMI);
	
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_DCMI);
  
  /* DCMI GPIO configuration **************************************************/
  /* D0..D4(PH9/10/11/12/14), HSYNC(PH8) */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | 
//                                GPIO_Pin_12 | GPIO_Pin_14| GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;  
  GPIO_Init(GPIOI, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9| GPIO_Pin_11;  //D0 D1 D4
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  //D5
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //HSYNC
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  //VSYNC
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
  /* PCLK(PA6) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* DCMI configuration *******************************************************/ 
  //DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_SnapShot;//DCMI_CaptureMode_Continuous
  DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;//DCMI_CaptureMode_Continuous
  DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;//DCMI_SynchroMode_Hardware
  DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;//DCMI_PCKPolarity_Falling
  DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_Low; //DCMI_VSPolarity_High
  DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low; //DCMI_HSPolarity_High
//  DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
//  DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_High;
  //DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_1of2_Frame;//DCMI_CaptureRate_All_Frame
  DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b; //?
  
  DCMI_Init(&DCMI_InitStructure);

	//DCMI_JPEGCmd(ENABLE);
	DCMI_JPEGCmd(DISABLE);   

//	DCMI_Crop.DCMI_CaptureCount = 			460-1;
//	DCMI_Crop.DCMI_HorizontalOffsetCount = 	20;
//	DCMI_Crop.DCMI_VerticalLineCount = 		320-1;
//	DCMI_Crop.DCMI_VerticalStartLine = 		0;
//	DCMI_CROPConfig (&DCMI_Crop);
//	DCMI_CROPCmd (ENABLE);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 

  /* DCMI Interrupts config ***************************************************/
	DCMI_ITConfig(DCMI_IT_VSYNC, ENABLE);
	DCMI_ITConfig(DCMI_IT_LINE,  ENABLE);
  DCMI_ITConfig(DCMI_IT_FRAME, ENABLE);
//   DCMI_ITConfig(DCMI_IT_ERR, ENABLE);
}



void DCMI_DMA_Init(uint32_t bufsize)
{
  DMA_InitTypeDef  DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configures the DMA2 to transfer Data from DCMI to the LCD ****************/
  /* Enable DMA2 clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
  
  /* DMA2 Stream1 Configuration */  
  DMA_DeInit(DMA2_Stream1);

  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;		
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) (DCMI_BUF_ADDRESS); 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = bufsize; 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//因为是传进数组，所以要启用自增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//DCMI读出的数据是32位的
  //DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//数组内的元素都是8位的
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//数组内的元素都是8位的
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;        
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  //DMA_SetCurrDataCounter(DMA2_Stream1, bufsize);
  //DMA_MemoryTargetConfig(DMA2_Stream1, (uint32_t) (DCMI_BUF_ADDRESS), DMA_Memory_0);
  DMA_DoubleBufferModeConfig (DMA2_Stream1, (uint32_t) (DCMI_BUF_ADDRESS + bufsize*4), DMA_Memory_0);
  DMA_DoubleBufferModeCmd (DMA2_Stream1, ENABLE);
	
  DMA_Init(DMA2_Stream1, &DMA_InitStructure); 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;                    
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
	DMA_ITConfig (DMA2_Stream1, DMA_IT_TC, ENABLE);		// enable transfer complete event
	
	
}



uint32_t Resolution_GetBufSize(ImageResolution_TypeDef ImageRes)
{
	uint32_t res = 0;
  switch(ImageRes)
  {
    case img_160x120:
    {
			res = 160*120;
      break;
    }
    case img_176x144:
    {
			res = 176*144;
			break;

    }
    case img_320x240:
    {
			res = 320*240;
      break;
    }
    case img_352x288:
    {
			res = 352*288;
      break;
    }
    case img_408x304:
    {
			res = 408*304;
      break;
    }
    case img_480x320:
    {
			res = 480*320;
      break;
    }
    case img_640x480:
    {
			res = 640*480;
      break;
    }
    case img_800x600:
    {
			res = 800*600;
      break;
    }    
	default:
    {
			res = 352*288;
      break;
    }
  }
	return res;
}


uint16_t Resolution_GetWidth(ImageResolution_TypeDef ImageRes)
{
	uint32_t res = 0;
  switch(ImageRes)
  {
    case img_160x120:
    {
			res = 160;
      break;
    }
    case img_176x144:
    {
			res = 176;
			break;

    }
    case img_320x240:
    {
			res = 320;
      break;
    }
    case img_352x288:
    {
			res = 352;
      break;
    }
    case img_408x304:
    {
			res = 408;
      break;
    }
    case img_480x320:
    {
			res = 480;
      break;
    }
    case img_640x480:
    {
			res = 640;
      break;
    }
    case img_800x600:
    {
			res = 800;
      break;
    }    
	default:
    {
			res = 352;
      break;
    }
  }
	return res;
}

uint16_t Resolution_GetHeight(ImageResolution_TypeDef ImageRes)
{
	uint32_t res = 0;
  switch(ImageRes)
  {
    case img_160x120:
    {
			res = 120;
      break;
    }
    case img_176x144:
    {
			res = 144;
			break;

    }
    case img_320x240:
    {
			res = 240;
      break;
    }
    case img_352x288:
    {
			res = 288;
      break;
    }
    case img_408x304:
    {
			res = 304;
      break;
    }
    case img_480x320:
    {
			res = 320;
      break;
    }
    case img_640x480:
    {
			res = 480;
      break;
    }
    case img_800x600:
    {
			res = 600;
      break;
    }
    default:
    {
			res = 288;
      break;
    }
  }
	return res;
}

/**
  * @}
  */
//static void Delay(uint32_t nTime)
//{
//  while(nTime--);
//} 

//static void Delay_ms(uint32_t nTime)
//{
//  while(nTime--)
//  	{Delay(1000);}
//}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
