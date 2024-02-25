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

typedef enum _X4_QEI_VEL_MODE_{
	QEI_RPM,
	QEI_RAD_S
}X4_QEI_ROT;

typedef enum _X4_QEI_DIST_MODE_{
	QEI_RAD,
	QEI_DEG
}X4_QEI_ANG;

typedef struct X4_QEI_t{
	int16_t PulseA, PulseB;
	int16_t curr, prev;
	int32_t pulse;
	int32_t PPR;
	uint32_t start_t, prev_t, dt;
	GPIO_TypeDef *PortA, *PortB;
	uint16_t PinA, PinB, PinZ;
	float RPM, RAD_S, MTR_S;
	float RAD, DEG, METER;

	void(*GetROT_VEL)(struct X4_QEI_t*, uint32_t);
	void(*GetLIN_VEL)(struct X4_QEI_t*, uint32_t, float);
	void(*GetANGLE)(struct X4_QEI_t*);
	void(*GetDIST)(struct X4_QEI_t*, float);
	void(*GetPulse)(struct X4_QEI_t*);
	void(*ResetQEI)(struct X4_QEI_t*);
}X4_QEI;

extern X4_QEI *Init_QEI(GPIO_TypeDef *PortA, GPIO_TypeDef *PortB,
		uint16_t PinA, uint16_t PinB, int32_t PPR,
		X4_QEI_ROT Vel, X4_QEI_ANG Ang);

#endif /* INC_QEI_X4_H_ */
