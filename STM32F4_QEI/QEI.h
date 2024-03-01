/*
 * QEI_X4.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Imaduddien
 */

#ifndef INC_QEI_X4_H_
#define INC_QEI_X4_H_

#include "stdint.h"
#include "stdlib.h"
#include "math.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#define QEI_PREV_MSK	(0x1)
#define QEI_CURR_MSK	(0x2)
#define QEI_INVALID		(0x3)

typedef struct STM32_QEI_t{
	uint16_t PulseA, PulseB;
	uint16_t curr, prev;
	int32_t pulse;
	int32_t PPR;
	uint32_t start_t, prev_t, dt;
	GPIO_TypeDef *PortA, *PortB;
	uint16_t PinA, PinB;
	GPIO_TypeDef *EXTI_Port, *POLL_Port;
	uint16_t EXTI_Pin, POLL_Pin;
	float RPM, RAD_S, MTR_S;
	float RAD, DEG, METER;

	void(*GetRPM)(struct STM32_QEI_t*, uint32_t);
	void(*GetRAD_S)(struct STM32_QEI_t*, uint32_t);
	void(*GetMTR_S)(struct STM32_QEI_t*, uint32_t, float);

	void(*GetDEG)(struct STM32_QEI_t*);
	void(*GetRAD)(struct STM32_QEI_t*);
	void(*GetMTR)(struct STM32_QEI_t*, float);

	void(*GetPulse)(struct STM32_QEI_t*);
	void(*ResetQEI)(struct STM32_QEI_t*);
}QEI;

extern QEI *Init_X4_QEI(GPIO_TypeDef *PortA, GPIO_TypeDef *PortB, uint16_t PinA, uint16_t PinB, int32_t PPR);
extern QEI *Init_X2_QEI(GPIO_TypeDef *EXTI_port, uint16_t EXTI_pin, GPIO_TypeDef *POLL_port, uint16_t POLL_pin, int32_t PPR);

#endif /* INC_QEI_X4_H_ */
