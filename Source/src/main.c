#include "main.h"

RCC_ClocksTypeDef RCC_Clocks;
extern volatile uint8_t LED_Toggle;
volatile int user_mode;

int main(void)
{ 
  /* Initialize LEDs */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
 
  /* Green Led On: start of application */
  STM_EVAL_LEDOn(LED4);
       
  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

  /* Initialize User Button */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

  WavePlayBack(I2S_AudioFreq_48k); 
  while (1);
}
