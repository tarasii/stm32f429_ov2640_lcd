/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DCMI_H
#define __DCMI_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


#define DCMI_DR_ADDRESS     0x50050028
//#define DCMI_BUF_ADDRESS     0xD0200000
#define DCMI_BUF_ADDRESS     0xD00EA600

#define FSMC_LCD_ADDRESS    0x60020000


typedef enum   
{
  img_160x120          =   0x01,	    /* Image 160x120 Size */
  img_176x144          =   0x02,	    /* Image 176x144 Size (QCIF) */
  img_320x240          =   0x03,	    /* Image 320x240 Size (QVGA) */
  img_352x288          =   0x04,	    /* Image 352x288 Size (CIF) */
  img_408x304          =   0x05,	    /* Image 408x304 Size */
  img_480x320          =   0x06,	    /* Image 480x320 Size */
  img_640x480          =   0x07	      /* Image 640x480 Size (VGA) */
}ImageResolution_TypeDef;

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Uncomment the line corresponding to the camera resolution */
//#define QVGA_SIZE  /*320x240 */
//#define VGA_SIZE  /* 640x480 */



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


void DCMI_GPIO_Init(void);
void DCMI_DMA_Init(uint32_t bufsize);	

uint32_t Resolution_GetBufSize(ImageResolution_TypeDef ImageRes);
uint16_t Resolution_GetWidth(ImageResolution_TypeDef ImageRes);
uint16_t Resolution_GetHeight(ImageResolution_TypeDef ImageRes);


#endif /* __DCMI_H */

