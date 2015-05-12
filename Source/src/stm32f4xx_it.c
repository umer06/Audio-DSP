/**
  ******************************************************************************
  * @file    Audio_playback_and_record/src/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28-October-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************/ 

#include "main.h"

volatile uint8_t LED_Toggle;
extern volatile int user_mode;

/**
  * @brief   This function handles NMI exception.
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

static void LED_periodic_controller (void)
{
    switch (LED_Toggle & LED_CTRL_RED_TOGGLE) {
        case LED_CTRL_RED_ON:
            STM_EVAL_LEDOn(LED5);
            break;
        case LED_CTRL_RED_OFF:
            STM_EVAL_LEDOff(LED5);
            break;
        case LED_CTRL_RED_TOGGLE:
            STM_EVAL_LEDToggle(LED5);
            break;
    }

    switch (LED_Toggle & LED_CTRL_ORANGE_TOGGLE) {
        case LED_CTRL_ORANGE_ON:
            STM_EVAL_LEDOn(LED3);
            break;
        case LED_CTRL_ORANGE_OFF:
            STM_EVAL_LEDOff(LED3);
            break;
        case LED_CTRL_ORANGE_TOGGLE:
            STM_EVAL_LEDToggle(LED3);
            break;
    }

    switch (LED_Toggle & LED_CTRL_GREEN_TOGGLE) {
        case LED_CTRL_GREEN_ON:
            STM_EVAL_LEDOn(LED4);
            break;
        case LED_CTRL_GREEN_OFF:
            STM_EVAL_LEDOff(LED4);
            break;
        case LED_CTRL_GREEN_TOGGLE:
            STM_EVAL_LEDToggle(LED4);
            break;
    }

    switch (LED_Toggle & LED_CTRL_BLUE_TOGGLE) {
        case LED_CTRL_BLUE_ON:
            STM_EVAL_LEDOn(LED6);
            break;
        case LED_CTRL_BLUE_OFF:
            STM_EVAL_LEDOff(LED6);
            break;
        case LED_CTRL_BLUE_TOGGLE:
            STM_EVAL_LEDToggle(LED6);
            break;
    }
}

volatile uint32_t msec_counter;

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    msec_counter++;

    // we use bit 1 of the user mode to control the LED toggling frequency

    if ((user_mode & 2) && !(msec_counter & 0x3f) ||
        !(user_mode & 2) && !(msec_counter & 0x7f))
            LED_periodic_controller ();
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  /* Check the clic on the accelerometer to Pause/Resume Playing */
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    STM_EVAL_LEDToggle(LED3);       // accel "click" toggles "orange"
    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

/**
  * @brief  EXTI0_IRQHandler
  *         This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  /* Checks whether the User Button EXTI line is asserted*/
  if (EXTI_GetITStatus(EXTI_Line0) != RESET) 
  { 
      static uint32_t last_button_time;     // used to debounce user button

      if (last_button_time + 200 < msec_counter) {
          if (++user_mode & 1)      // bit 0 of the user mode lights the blue LED
              LED_Toggle = (LED_Toggle & ~LED_CTRL_BLUE_TOGGLE) | LED_CTRL_BLUE_ON;
          else
              LED_Toggle = (LED_Toggle & ~LED_CTRL_BLUE_TOGGLE) | LED_CTRL_BLUE_OFF;

          last_button_time = msec_counter;
      }
  } 
  /* Clears the EXTI's line pending bit.*/ 
  EXTI_ClearITPendingBit(EXTI_Line0);
}

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
  
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
