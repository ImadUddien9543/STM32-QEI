/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "QEI.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN 0 */
QEI *EncoderX2, *EncoderX4;
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
  EncoderX2 = Init_X2_QEI(CH_A_GPIO_Port, CH_A_Pin, CH_B_GPIO_Port, CH_B_Pin, 360);
  EncoderX4 = Init_X4_QEI(CH_A_GPIO_Port, CH_B_GPIO_Port, CH_A_Pin, CH_B_Pin, 360);
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
	  Encoder2->GetMTR_S(Encoder2, sample_time, wheel_r);
	  Encoder2->GetMTR(Encoder2, wheel_r);

	  Degree = Encoder1->DEG;
	  EncRPM = Encoder1->RPM;
	  LinDistance = Encoder2->METER;
	  LinVel = Encoder2->MTR_S;
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == EncoderX4->PinA) EncoderX4->GetPulse(EncoderX4);
	else if(GPIO_Pin == EncoderX4->PinB) EncoderX4->GetPulse(EncoderX4);
	else if(GPIO_Pin == EncoderX2->EXTI_Pin) EncoderX2->GetPulse(EncoderX2);
	else __NOP();
}
/* USER CODE END 4 */
