/** Taras Ivaniv **/

@par Example Description

This example fully based on WaveShare and STMicroelectronics examples
Example shows how to make OV2640 camera 640x480 output on 800x480 LCD with Open429I-C devboard.
DCMI uses DMA doublebuffer mode. (Max resolution without double buffer is 408x302)
Example runs on: 
WaveShare Open4X9I-C devboard with STM32F429ITG 8MHz 
OV2640 with 50MHz external crystal generator 

Example uses STM32F4xx_DSP_StdPeriph_Lib_V1.6.1 
Just git clone in STM32F4xx_DSP_StdPeriph_Lib_V1.6.1\Project\ folder

The USARTx is configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled


@par Directory contents 
  
  - system_stm32f4xx.c   STM32F4xx system clock configuration file
  - stm32f4xx_conf.h     Library Configuration file
  - stm32f4xx_it.c       Interrupt handlers
  - stm32f4xx_it.h       Interrupt handlers header file
  - stm32f429_lcd.c      LCD config and graphics module
  - stm32f429_lcd.h      LCD header file
  - stm32f429_sdram.c    SDRAM config module
  - stm32f429_sdram.h    SDRAM header file
  - main.c               Main program
  - main.h               Main program header file
  - dcmi.c               DCMI GPIO DMA config module
  - dcmi.h               DCMI header file
  - usart.c              USART config module
  - usart.h              USART header file
  - i2c.c                I2C module
  - i2c.h                I2C header file
  
     

tarasii@gmail.com
http://www.waveshare.com/product/mcu-tools/stm32/open.htm
http://www.waveshare.com/wiki/Open429I-C
http://org-www.st.com/en/microcontrollers.html
  

