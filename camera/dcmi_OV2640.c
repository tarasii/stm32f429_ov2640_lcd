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
#include "camera/dcmi_OV2640.h"
#include "camera/DCMI_OV2640_INITTABLE.h"

/** @addtogroup DCMI_OV2640_Camera
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Bits definitions ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Delay(uint32_t nTime); 
static void Delay_ms(uint32_t nTime);

/**
  * @brief  Configures the DCMI to interface with the OV2640 camera module.
  * @param  None
  * @retval None
  */
	
void EXTI_Config(void)     
{
  GPIO_InitTypeDef   GPIO_InitStructure;  
  NVIC_InitTypeDef   NVIC_InitStructure;   
	EXTI_InitTypeDef   EXTI_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);   

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;   
  GPIO_Init(GPIOB, &GPIO_InitStructure);    
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);
 
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line15;//PB15做外部中断,下降沿触发,做为拍照按钮
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;     
  EXTI_Init(&EXTI_InitStructure);  

 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}
	
void OV2640_DCMI_Config(void)
{
 DCMI_InitTypeDef DCMI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
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
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* DCMI configuration *******************************************************/ 
  //DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_SnapShot;//DCMI_CaptureMode_Continuous
  DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;//DCMI_CaptureMode_Continuous
  DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;//DCMI_SynchroMode_Hardware
  DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;//DCMI_PCKPolarity_Falling
  DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_Low; //DCMI_VSPolarity_High
  DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low; //DCMI_HSPolarity_High
  DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;//DCMI_CaptureRate_All_Frame
  DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b; //?
  
  DCMI_Init(&DCMI_InitStructure);

	//DCMI_JPEGCmd(ENABLE);
	DCMI_JPEGCmd(DISABLE);   
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 

  /* DCMI Interrupts config ***************************************************/
//	 DCMI_ITConfig(DCMI_IT_VSYNC, ENABLE);
//	 DCMI_ITConfig(DCMI_IT_LINE, ENABLE);
     DCMI_ITConfig(DCMI_IT_FRAME, ENABLE);
//   DCMI_ITConfig(DCMI_IT_ERR, ENABLE);
}


/**
  * @brief  Set PA8 Output SYSCLK/2.
  * @param  None
  * @retval None
  */
void MCO1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_ClockSecuritySystemCmd(ENABLE);

	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);
		
	/* Configure MCO (PA8) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  //UP
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_MCO1Config(RCC_MCO1Source_HSI, RCC_MCO1Div_1);// 16MHZ
}

void DCMI_0V2640_PWDN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*PWDN*/
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	Delay_ms(10);
	GPIO_SetBits(GPIOA, GPIO_Pin_2);	
}

void OV2640_DMA_Init(uint32_t bufsize)
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
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//数组内的元素都是8位的
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;        
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
     
  DMA_Init(DMA2_Stream1, &DMA_InitStructure); 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;                    
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}

void OV2640_Init(uint32_t bufsize)
{
	SCCB_GPIO_Config();
	MCO1_Init();
	OV2640_DCMI_Config();
	OV2640_DMA_Init(bufsize);
	OV2640_Reset();
  Delay_ms(0xfff);
}


void OV2640_Reset(void)
{
  DCMI_SingleRandomWrite(OV2640_DSP_RA_DLMT, 0x01);
  DCMI_SingleRandomWrite(OV2640_SENSOR_COM7, 0x80);
}
/**
  * @brief  Read the OV2640 Manufacturer identifier.
  * @param  OV2640ID: pointer to the OV2640 Manufacturer identifier. 
  * @retval None
  */
uint8_t DCMI_OV2640_ReadID(OV2640_IDTypeDef* OV2640ID)
{
	uint8_t temp;
	DCMI_SingleRandomWrite(OV2640_DSP_RA_DLMT, 0x01);
	if(DCMI_SingleRandomRead(OV2640_SENSOR_MIDH,&temp)!=0)
		return 0xff;
	OV2640ID->Manufacturer_ID1 = temp;
	if(DCMI_SingleRandomRead(OV2640_SENSOR_MIDL,&temp)!=0)
		return 0xff;
	OV2640ID->Manufacturer_ID2 = temp;
	if(DCMI_SingleRandomRead(OV2640_SENSOR_PIDH,&temp)!=0)
		return 0xff;
	OV2640ID->Version = temp;
	if(DCMI_SingleRandomRead(OV2640_SENSOR_PIDL,&temp)!=0)
		return 0xff;
	OV2640ID->PID = temp;

	return 0;
}



/**
  * @brief  Configures the OV2640 camera in JPEG mode.
  * @param  JPEGImageSize: JPEG image size
  * @retval None
  */
void OV2640_Config(OV2640_ImageResolution_TypeDef ImageRes)
{
  uint32_t i;

  OV2640_Reset();
  Delay_ms(200);

  for(i=0; i<(sizeof(OV2640_INIT)/2); i++)
  {
    DCMI_SingleRandomWrite(OV2640_INIT[i][0], OV2640_INIT[i][1]);
		Delay_ms(1);
  }
	
  for(i=0; i<(sizeof(OV2640_YUV422)/2); i++)
  {
    DCMI_SingleRandomWrite(OV2640_YUV422[i][0], OV2640_YUV422[i][1]);
		Delay_ms(1);
  }

  Delay_ms(100);
		
	for(i=0; i<(sizeof(OV2640_SENS_INIT)/2); i++)
	{
		DCMI_SingleRandomWrite(OV2640_SENS_INIT[i][0], OV2640_SENS_INIT[i][1]);
	}
	
  switch(ImageRes)
  {
    case img_160x120:
    {
			for(i=0; i<(sizeof(OV2640_DSP_160x120)/2); i++)
      {
        DCMI_SingleRandomWrite(OV2640_DSP_160x120[i][0], OV2640_DSP_160x120[i][1]);
      }      
      break;
    }
    case img_176x144:
    {
			for(i=0; i<(sizeof(OV2640_DSP_176x144)/2); i++)
      {
        DCMI_SingleRandomWrite(OV2640_DSP_176x144[i][0], OV2640_DSP_176x144[i][1]);
      }      
			break;
    }
    case img_320x240:
    {
			for(i=0; i<(sizeof(OV2640_DSP_320x200)/2); i++)
      {
        DCMI_SingleRandomWrite(OV2640_DSP_320x200[i][0], OV2640_DSP_320x200[i][1]);
      }
      break;
    }
    case img_352x288:
    {
			for(i=0; i<(sizeof(OV2640_DSP_352x288)/2); i++)
      {
        DCMI_SingleRandomWrite(OV2640_DSP_352x288[i][0], OV2640_DSP_352x288[i][1]);
      }		
      break;
    }
    case img_408x304:
    {
			for(i=0; i<(sizeof(OV2640_DSP_408x304)/2); i++)
      {
        DCMI_SingleRandomWrite(OV2640_DSP_408x304[i][0], OV2640_DSP_408x304[i][1]);
      }		
      break;
    }
    case img_480x320:
    {
			for(i=0; i<(sizeof(OV2640_DSP_480x320)/2); i++)
      {
        DCMI_SingleRandomWrite(OV2640_DSP_480x320[i][0], OV2640_DSP_480x320[i][1]);
      }		
      break;
    }
    case img_640x480:
    {
			for(i=0; i<(sizeof(OV2640_DSP_640x480)/2); i++)
      {
        DCMI_SingleRandomWrite(OV2640_DSP_640x480[i][0], OV2640_DSP_640x480[i][1]);
      }		
      break;
    }
    default:
    {
			for(i=0; i<(sizeof(OV2640_DSP_352x288)/2); i++)
      {
        DCMI_SingleRandomWrite(OV2640_DSP_352x288[i][0], OV2640_DSP_352x288[i][1]);
      }		
      break;
    }
  }
}

void SCCB_WriteRegs(const uint8_t* pbuf)
{
	while(1)
	{
		if((*pbuf == 0) && (*(pbuf + 1) == 0))
		{
			break;
		}
		else
		{
			DCMI_SingleRandomWrite(*pbuf++, *pbuf++);
		}
	}
}


const static uint8_t OV2640_AUTOEXPOSURE_LEVEL0[]=
{
	0xFF,	0x01,	0xff,
	0x24,	0x20,	0xff,
	0x25,	0x18,	0xff,
	0x26,	0x60,	0xff,
	0x00,	0x00,	0x00
};

const static uint8_t OV2640_AUTOEXPOSURE_LEVEL1[]=
{
	0xFF,	0x01,	0xff,
	0x24,	0x34,	0xff,
	0x25,	0x1c,	0xff,
	0x26,	0x70,	0xff,
	0x00,	0x00,	0x00
};
const static uint8_t OV2640_AUTOEXPOSURE_LEVEL2[]=
{
	0xFF,	0x01,	0xff,
	0x24,	0x3e,	0xff,
	0x25,	0x38,	0xff,
	0x26,	0x81,	0xff,
	0x00,	0x00,	0x00
};
const static uint8_t OV2640_AUTOEXPOSURE_LEVEL3[]=
{
	0xFF,	0x01,	0xff,
	0x24,	0x48,	0xff,
	0x25,	0x40,	0xff,
	0x26,	0x81,	0xff,
	0x00,	0x00,	0x00
};
const static uint8_t OV2640_AUTOEXPOSURE_LEVEL4[]=
{
	0xFF,	0x01,	0xff,
	0x24,	0x58,	0xff,
	0x25,	0x50,	0xff,
	0x26,	0x92,	0xff,
	0x00,	0x00,	0x00
};

void OV2640_AutoExposure(uint8_t level)
{
	switch(level)
	{
		case 0:
			SCCB_WriteRegs(OV2640_AUTOEXPOSURE_LEVEL0);
			break;
		case 1:
			SCCB_WriteRegs(OV2640_AUTOEXPOSURE_LEVEL1);
			break;
		case 2:
			SCCB_WriteRegs(OV2640_AUTOEXPOSURE_LEVEL2);
			break;
		case 3:
			SCCB_WriteRegs(OV2640_AUTOEXPOSURE_LEVEL3);
			break;
		case 4:
			SCCB_WriteRegs(OV2640_AUTOEXPOSURE_LEVEL4);
			break;
		default:
			SCCB_WriteRegs(OV2640_AUTOEXPOSURE_LEVEL0);
			break;
	}
	
}


void OV2640_BrightnessConfig(uint8_t Brightness)
{
  DCMI_SingleRandomWrite(0xff, 0x00);
  DCMI_SingleRandomWrite(0x7c, 0x00);
  DCMI_SingleRandomWrite(0x7d, 0x04);
  DCMI_SingleRandomWrite(0x7c, 0x09);
  DCMI_SingleRandomWrite(0x7d, Brightness);
  DCMI_SingleRandomWrite(0x7d, 0x00);
}


uint32_t OV2640_GetBufSize(OV2640_ImageResolution_TypeDef ImageRes)
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
    default:
    {
			res = 352*288;
      break;
    }
  }
	return res;
}

uint16_t OV2640_GetWidth(OV2640_ImageResolution_TypeDef ImageRes)
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
    default:
    {
			res = 352;
      break;
    }
  }
	return res;
}

uint16_t OV2640_GetHeight(OV2640_ImageResolution_TypeDef ImageRes)
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
static void Delay(uint32_t nTime)
{
  while(nTime--);
} 

static void Delay_ms(uint32_t nTime)
{
  while(nTime--)
  	{Delay(1000);}
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
