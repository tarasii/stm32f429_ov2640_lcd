/**
  ******************************************************************************
  * @file    USART/USART_Printf/main.c 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    13-November-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "usart.h"
//#include "dcmi.h"
#include "dcmi_OV2640.h"
#include "32f429_lcd.h"
#include "32f429_sdram.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
static __IO uint32_t TimingDelay;

void LCD_DisplayByte(uint16_t lnnum, uint16_t pos, uint8_t byte);
void LCD_DisplayRawLine(uint16_t lnnum, uint8_t *buf);
void LCD_DisplayBuf(uint8_t *buf, ImageResolution_TypeDef ImageRes);


uint8_t jpg_flag=0;
uint8_t key_flag=0;


#define GPIO_WAKEUP_CLK    RCC_AHB1Periph_GPIOA
#define GPIO_WAKEUP_PORT   GPIOA
#define GPIO_WAKEUP_PIN    GPIO_Pin_0

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40_41xxx.s/startup_stm32f427_437xx.s/startup_stm32f429_439xx.s)
       before to branch to application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file.
     */     
	GPIO_InitTypeDef GPIO_InitStructure;
	ImageResolution_TypeDef cur_img_res;
	OV2640_IDTypeDef OV2640ID;
	RCC_ClocksTypeDef SYS_Clocks;
	//uint32_t i=0;
	
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
  /* USART configuration */
  USART_Config();
       
  /* Output a message on Hyperterminal using printf function */
  
  RCC_GetClocksFreq(&SYS_Clocks);
		
	USART_SendData(USART1, (uint8_t) 'Z');
		
	printf("\r\nSYSCLK:%dM\r\n",SYS_Clocks.SYSCLK_Frequency/1000000);
	printf("HCLK:%dM\r\n",SYS_Clocks.HCLK_Frequency/1000000);
	printf("PCLK1:%dM\r\n",SYS_Clocks.PCLK1_Frequency/1000000);
	printf("PCLK2:%dM\r\n",SYS_Clocks.PCLK2_Frequency/1000000);	
	printf("\n\r DCMI Example\n\r");
		
  LCD_Init();
  
  /* LCD Layer initialization */
  LCD_LayerInit();
    
  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);


	LCD_SetLayer(LCD_BACKGROUND_LAYER);
	//LCD_SetLayer(LCD_FOREGROUND_LAYER);		
	LCD_Clear(LCD_COLOR_BLACK);
	
	LCD_SetTextColor(LCD_COLOR_GREEN);	
	//cur_img_res = img_160x120;
	//cur_img_res = img_480x320;	
	cur_img_res = img_408x304;
	LCD_DrawRect(196-1, 20-1, Resolution_GetHeight(cur_img_res)+1, Resolution_GetWidth(cur_img_res)+1);	
	
	//LCD_SetLayer(LCD_FOREGROUND_LAYER);	
	LCD_SetFont(&Font8x12);
	LCD_SetTextColor(LCD_COLOR_GREEN);
	LCD_SetBackColor(LCD_COLOR_BLACK);
	LCD_DisplayStringLine(LINE(0), (uint8_t*)"0123456789012345678901234567890123456789012345678901234567890"); 
	LCD_DisplayStringLine(LINE(1), (uint8_t*)"  test "); 
	LCD_DrawRect(0,0,LCD_PIXEL_HEIGHT-1,LCD_PIXEL_WIDTH-1);

	
	OV2640_Init(Resolution_GetBufSize(cur_img_res)/2);
	Delay(1);		
	if(DCMI_OV2640_ReadID(&OV2640ID)==0)
	{	
		if(OV2640ID.Manufacturer_ID1==0x7f && OV2640ID.Manufacturer_ID2==0xa2 
			&& OV2640ID.Version==0x26 && OV2640ID.PID==0x42){
 			printf("OV2640 ID:0x%x 0x%x 0x%x 0x%x\r\n",
 				OV2640ID.Manufacturer_ID1, OV2640ID.Manufacturer_ID2, OV2640ID.PID, OV2640ID.Version);
		}
		else{
 			printf("OV2640 ID is Error!\r\n");
		}			
	}	
	
	OV2640_Config(cur_img_res);
	OV2640_BrightnessConfig(0x20);	
	OV2640_AutoExposure(2);	
	Delay(10);
	
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_WAKEUP_PIN;
	GPIO_Init(GPIO_WAKEUP_PORT, &GPIO_InitStructure);

	DMA_Cmd(DMA2_Stream1, ENABLE);		
	DCMI_Cmd(ENABLE);
	DCMI_CaptureCmd(ENABLE);	
	
  while (1)
  {
//	if(GPIO_ReadInputDataBit(GPIO_WAKEUP_PORT,GPIO_WAKEUP_PIN))
//		{
//		 key_flag = 1;

//	    DMA_Cmd(DMA2_Stream1, ENABLE);		
//			DCMI_Cmd(ENABLE);
//			DCMI_CaptureCmd(ENABLE);
//			//printf("key_flag =1 \r\n");
//			while(GPIO_ReadInputDataBit(GPIO_WAKEUP_PORT,GPIO_WAKEUP_PIN));
//		}
//		
//		
//		
		if(jpg_flag)//&&key_flag
		{
//			DCMI_Cmd(DISABLE); 
//			DCMI_CaptureCmd(DISABLE);
//			DMA_Cmd(DMA2_Stream1, DISABLE);

			jpg_flag = 0;
//			key_flag = 0;	
//			
////			LCD_Clear(LCD_COLOR_BLACK);					
////			//LCD_DisplayStringLine(LINE(0), (uint8_t*)"0001020304050607080910111213141516171819202122232425262728293031"); 

////			for (i = 0; i < 40; i++){
////				//LCD_DisplayRawLine(LINE(i), JpegBuffer+(16*i));				
////				//LCD_DisplayRawLine(LINE(i+1), (uint8_t *)(DCMI_DR_ADDRESS +32*i));				
////				LCD_DisplayRawLine(LINE(i+1), (uint8_t *)(0xD0200000 +32*i));				
////			}

//			//LCD_DisplayBuf(  (uint8_t *) (DCMI_BUF_ADDRESS), cur_img_res);
//			printf("Frame!");

//	    DMA_Cmd(DMA2_Stream1, ENABLE);		
//			DCMI_Cmd(ENABLE);
//			DCMI_CaptureCmd(ENABLE);
//			
////		 jpg_flag = 1;
////		} else {
////			if (key_flag) {
////			}else {
////				key_flag = 1;
////				DMA_Cmd(DMA2_Stream1, ENABLE);		
////				DCMI_Cmd(ENABLE);
////				DCMI_CaptureCmd(ENABLE);
////			}
		}
  }
}

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0)
  {}
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void LCD_DisplayByte(uint16_t lnnum, uint16_t pos, uint8_t byte)
{
	uint8_t tmp;
	tmp = byte & 0x0f;
	if (tmp > 9){
		tmp = 0x37 + tmp;
	} else {
		tmp = 0x30 + tmp;
	}
	LCD_DisplayChar(lnnum, pos, tmp);
	tmp = (byte & 0xf0)>>4;
	if (tmp > 9){
		tmp = 0x37 + tmp;
	} else {
		tmp = 0x30 + tmp;
	}
	LCD_DisplayChar(lnnum, pos+8, tmp);
}

void LCD_DisplayRawLine(uint16_t lnnum, uint8_t *buf)
{
	uint8_t i;
	for(i=0;i<32;i++){
		LCD_DisplayByte(lnnum, i*16, *(buf+i));
	}
}

void LCD_DisplayBuf(uint8_t *buf, ImageResolution_TypeDef ImageRes)
{
	uint16_t i,j;	
	__IO uint16_t tmp;
	uint32_t offs, joffs;
	uint16_t xlen = Resolution_GetWidth(ImageRes), ylen = Resolution_GetHeight(ImageRes);
	uint32_t LCD_ofset_buf = LCD_FRAME_BUFFER + BUFFER_OFFSET;
	//uint32_t LCD_ofset_buf = LCD_FRAME_BUFFER;

	for(j=0; j<ylen; j++){
		joffs = 2 * j * xlen;
		offs = LCD_ofset_buf + j * LCD_PIXEL_WIDTH * 2;
//		offs = LCD_ofset_buf + j * 408 * 2;
		for(i=0; i<xlen; i++){
			tmp = *(__IO uint16_t *) (buf + 2*i + joffs);
			*(__IO uint16_t *) (2*i + offs) = tmp;
		}
	}
}

void LCD_DisplayBufOld(uint8_t *buf, ImageResolution_TypeDef ImageRes)
{
	uint16_t i,j;	
	uint16_t tmp;
	uint32_t offs, joffs;
	uint16_t xlen = Resolution_GetWidth(ImageRes), ylen = Resolution_GetHeight(ImageRes);
	uint32_t LCD_ofset_buf = LCD_FRAME_BUFFER + BUFFER_OFFSET;
	//uint32_t LCD_ofset_buf = LCD_FRAME_BUFFER;

	for(j=0; j<ylen; j++){
		joffs = 2 * j * xlen;
		offs = LCD_ofset_buf + j * LCD_PIXEL_WIDTH * 2;
//		offs = CFBuf + j * 408 * 2;
		for(i=0;i<xlen;i++){
			//*(__IO uint16_t *) (offs + i * 2) = LCD_COLOR_RED;
			tmp = (*(buf + 2 * i + joffs) << 8) | *(buf + 2 * i + joffs + 1) ;
			//tmp = (*(buf + 2 * i + joffs + 1) << 8) | *(buf + 2 * i + joffs) ;
			*(__IO uint16_t *) (offs + i * 2) = tmp;
		}
	}
}

void LCD_CopyBufToMem(uint8_t *buf, ImageResolution_TypeDef ImageRes)
{
	uint16_t i;	
	__IO uint32_t tmp;
	uint32_t LCD_ofset_buf = LCD_FRAME_BUFFER + BUFFER_OFFSET;
	uint16_t xlen = Resolution_GetWidth(ImageRes), ylen = Resolution_GetHeight(ImageRes);

	for(i=0; i<xlen*ylen/2; i++){
		tmp = *(__IO uint32_t *) (buf + 4*i);
		*(__IO uint32_t *) (LCD_ofset_buf + 4*i) = tmp;
	}
}

void LCD_CopyBufToMemDMA(uint8_t *buf)
{
//	DMA_InitTypeDef  DMA_InitStructure;
//	DMA_DeInit(DMA1_Channel1);
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//	DMA_InitStructure.DMA_BufferSize = OV2640_GetWidth(ImageRes)/2;
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)buf;
//	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(LCD_FRAME_BUFFER + BUFFER_OFFSET);
//	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
//	 
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {

  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
