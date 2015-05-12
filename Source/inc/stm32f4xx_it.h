/**
  ******************************************************************************
  * @file    Audio_playback_and_record/inc/stm32f40x_it.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28-October-2011
  * @brief   This file contains the headers of the interrupt handlers.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F40x_IT_H
#define __STM32F40x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#define LED_CTRL_RED_OFF       0x01
#define LED_CTRL_RED_ON        0x02
#define LED_CTRL_RED_TOGGLE    0x03

#define LED_CTRL_ORANGE_OFF    0x04
#define LED_CTRL_ORANGE_ON     0x08
#define LED_CTRL_ORANGE_TOGGLE 0x0C

#define LED_CTRL_GREEN_OFF     0x10
#define LED_CTRL_GREEN_ON      0x20
#define LED_CTRL_GREEN_TOGGLE  0x30

#define LED_CTRL_BLUE_OFF      0x40
#define LED_CTRL_BLUE_ON       0x80
#define LED_CTRL_BLUE_TOGGLE   0xC0

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void RTC_Alarm_IRQHandler(void);
void TIM2_IRQHandler(void);
extern void USB_OTG_BSP_TimerIRQ(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F40x_IT_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
