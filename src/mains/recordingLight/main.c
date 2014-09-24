/* Includes ------------------------------------------------------------------*/
#include "System.h"

#include "stm32f0xx.h"
#include "RecordingLight.h"

/*#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"

#include "Tim1_Pwm.h"

static uint8_t rgb[3] = {255, 0, 0};

typedef struct {
	uint8_t index;
	char delta;
} stepDesc;

static stepDesc steps[] = {{1,1}, {0,-1}, {2,1}, {1,-1}, {0,1}, {2,-1}};
static uint8_t currentStepIndex = 0;

void tick(uint32_t time){
	static uint32_t ctr;
	static uint8_t stepCounter = 0;

	if (ctr % 20 == 0)
	{
		rgb[steps[currentStepIndex].index] += steps[currentStepIndex].delta;
		stepCounter++;

		if (stepCounter==255){
			stepCounter=0;
			currentStepIndex++;
			if (currentStepIndex==6){
				currentStepIndex = 0;
			}
		}

		Tim1_Pwm_setValueCh1(rgb[0]);
		Tim1_Pwm_setValueCh2(rgb[1]);
		Tim1_Pwm_setValueCh3(rgb[2]);
	}

	ctr++;
}

*/

int main(void) {
	System_init();

	System_PITSetup(1);

	RecordingLight_init();
	System_PITSetTaskCallback(&RecordingLight_tick);

	//System_PITSetTaskCallback(&tick);

	System_PITStart();

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
