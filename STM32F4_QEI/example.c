/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "QEI_X4.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN 0 */
X4_QEI *Encoder1;
float LinDistance, LinVel;
float EncRPM, Degree;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 2 */
  Encoder1 = Init_QEI(CH_A_GPIO_Port, CH_B_GPIO_Port, CH_A_Pin, CH_B_Pin, 360);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  float wheel_r = 10;
	  uint32_t sample_time = 100;
	  Encoder1->GetDEG(Encoder1);
	  Encoder1->GetRPM(Encoder1, sample_time);
	  Encoder1->GetMTR_S(Encoder1, sample_time, wheel_r);
	  Encoder1->GetMTR(Encoder1, wheel_r);

	  Degree = Encoder1->DEG;
	  EncRPM = Encoder1->RPM;
	  LinDistance = Encoder1->METER;
	  LinVel = Encoder1->MTR_S;
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(Encoder1->PinA || Encoder1->PinB) Encoder1->GetPulse(Encoder1);
	else __NOP();
}
/* USER CODE END 4 */
