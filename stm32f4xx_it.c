/**
  ******************************************************************************
  * @file    USART/USART_Printf/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    13-November-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f4xx_it.h"
#include "pin.h"
#include "dcmi.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void TimingDelay_Decrement(void);
void SysTick_Handler(void)
{
		TimingDelay_Decrement();
}
extern uint8_t jpg_flag;
extern uint8_t key_flag;
extern uint8_t dcim_flag;
extern uint16_t lncnt;
extern uint16_t lnmax;
extern uint16_t fps;
void DCMI_IRQHandler(void)
{  	   
	if (DCMI_GetITStatus(DCMI_IT_FRAME) != RESET) {		
		DCMI_ClearITPendingBit(DCMI_IT_FRAME); 	
		GPIO_TOGGLE(LED1_GPIO_Port, LED1_Pin);		
		dcim_flag = 0;
		fps++;
	}

	if (DCMI_GetITStatus(DCMI_IT_LINE) != RESET){
		DCMI_ClearFlag(DCMI_FLAG_LINERI);
		GPIO_TOGGLE(LED2_GPIO_Port, LED2_Pin);
		lncnt++;
	}
	
	if (DCMI_GetITStatus(DCMI_IT_VSYNC) != RESET){
		DCMI_ClearFlag(DCMI_FLAG_VSYNCRI);
		GPIO_TOGGLE(LED3_GPIO_Port, LED3_Pin);
    lnmax = lncnt;
		lncnt = 0;		
	}
	
}

void DMA2_Stream1_IRQHandler (void)
{
	uint32_t cur_mb;
	cur_mb = DMA_GetCurrentMemoryTarget(DMA2_Stream1);
	if (cur_mb == DMA_Memory_0) cur_mb = DMA_Memory_1; else cur_mb = DMA_Memory_0;
	
	if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1) == SET)
	{
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
		GPIO_TOGGLE(LED4_GPIO_Port, LED4_Pin);
		if (dcim_flag == 0) {
				DMA_MemoryTargetConfig(DMA2_Stream1, (uint32_t) (DCMI_BUF_ADDRESS + 640 * 480), cur_mb);
		}
		if (dcim_flag == 1) {
				DMA_MemoryTargetConfig(DMA2_Stream1, (uint32_t) (DCMI_BUF_ADDRESS + 640 * 480 * 3 / 2), cur_mb);
		}
		if (dcim_flag == 2) {
				DMA_MemoryTargetConfig(DMA2_Stream1, (uint32_t) (DCMI_BUF_ADDRESS), cur_mb);
		}
		if (dcim_flag == 3) {
				DMA_MemoryTargetConfig(DMA2_Stream1, (uint32_t) (DCMI_BUF_ADDRESS + 640 * 480 / 2), cur_mb);
		}
		dcim_flag++;
		if (dcim_flag > 3) dcim_flag = 0;
	}
}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
