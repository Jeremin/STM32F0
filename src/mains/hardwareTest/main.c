/* Includes ------------------------------------------------------------------*/
#include "System.h"

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_tim.h"

int main(void) {
	GPIO_InitTypeDef gpioInit;
	TIM_TimeBaseInitTypeDef timerTimeBaseInit;
	TIM_OCInitTypeDef timerOCInit;

	System_init();

/*	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDOn(LED3);
	while(1);*/

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioInit);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_1);

	timerTimeBaseInit.TIM_Prescaler = 0;
	timerTimeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
	timerTimeBaseInit.TIM_Period = 500;
	timerTimeBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
	timerTimeBaseInit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timerTimeBaseInit);

	timerOCInit.TIM_OCMode = TIM_OCMode_PWM1;
	timerOCInit.TIM_OutputState = TIM_OutputState_Enable;
	timerOCInit.TIM_OutputNState = TIM_OutputNState_Enable;
	timerOCInit.TIM_OCPolarity = TIM_OCPolarity_Low;
	timerOCInit.TIM_OCNPolarity = TIM_OCNPolarity_High;
	timerOCInit.TIM_OCIdleState = TIM_OCIdleState_Set;
	timerOCInit.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	timerOCInit.TIM_Pulse = 400;

	TIM_OC3Init(TIM3, &timerOCInit);

	TIM_Cmd(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);

	while (1) {

	}
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
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
	{}
}
#endif
