/*
 * QEI.h
 *
 *  Created on: Feb 25, 2024
 *      Author: Imaduddien
 */

#include "QEI_X4.h"

static void Encode_X4_QEI(QEI *self);
static void Encode_X2_QEI(QEI *self);
static void ResetQEI(QEI *self);

static void GetRPM(QEI *self, uint32_t sample_t);
static void GetRAD_S(QEI *self, uint32_t sample_t);
static void GetMTR_S(QEI *self, uint32_t sample_t, float r);

static void GetDEG(QEI *self);
static void GetRAD(QEI *self);
static void GetMTR(QEI *self, float r);

QEI *Init_X4_QEI(GPIO_TypeDef *PortA, GPIO_TypeDef *PortB, uint16_t PinA, uint16_t PinB, int32_t PPR){
	QEI *self;
	self = (QEI*)malloc(sizeof(QEI));

	self->PortA = PortA;
	self->PortB = PortB;
	self->PinA = PinA;
	self->PinB = PinB;
	self->PPR = PPR;

	self->pulse = 0;
	self->PulseA = 0;
	self->PulseB = 0;

	self->RPM = self->RAD_S = self->MTR_S = 0;
	self->RAD = self->DEG = self->METER = 0;

	uint16_t temp_pulseA = (uint16_t)((self->PortA->IDR & self->PinA) == self->PinA);
	uint16_t temp_pulseB = (uint16_t)((self->PortA->IDR & self->PinA) == self->PinA);
	self->curr = (temp_pulseA << 1) | (temp_pulseB);
	self->prev = self->curr;

	self->GetPulse = Encode_X4_QEI;
	self->ResetQEI = ResetQEI;
	self->GetRAD_S = GetRAD_S;
	self->GetRPM = GetRPM;
	self->GetMTR_S = GetMTR_S;
	self->GetRAD = GetRAD;
	self->GetDEG = GetDEG;
	self->GetMTR = GetMTR;

	return self;
}

QEI *Init_X2_QEI(GPIO_TypeDef *EXTI_port, uint16_t EXTI_pin, GPIO_TypeDef *POLL_port, uint16_t POLL_pin, int32_t PPR){
	QEI *self;
	self = (QEI*)malloc(sizeof(QEI));

	self->EXTI_Port = EXTI_port;
	self->POLL_Port = POLL_port;
	self->EXTI_Pin = EXTI_pin;
	self->POLL_Pin = POLL_pin;

	self->PPR = PPR;
	self->pulse = 0;
	self->PulseA = 0;
	self->PulseB = 0;

	self->RPM = self->RAD_S = self->MTR_S = 0;
	self->RAD = self->DEG = self->METER = 0;

	uint16_t temp_pulseA = (uint16_t)((self->EXTI_Port->IDR & self->EXTI_Pin) == self->EXTI_Pin);
	uint16_t temp_pulseB = (uint16_t)((self->POLL_Port->IDR & self->POLL_Pin) == self->POLL_Pin);
	self->curr = (temp_pulseA << 1) | (temp_pulseB);
	self->prev = self->curr;

	self->GetPulse = Encode_X2_QEI;
	self->ResetQEI = ResetQEI;
	self->GetRAD_S = GetRAD_S;
	self->GetRPM = GetRPM;
	self->GetMTR_S = GetMTR_S;
	self->GetRAD = GetRAD;
	self->GetDEG = GetDEG;
	self->GetMTR = GetMTR;

	return self;
}

static void Encode_X4_QEI(QEI *self){
	int direction = 0; //1 = counter-clockwise, else clockwise
	self->PulseA = (uint16_t)((self->PortA->IDR & self->PinA) == self->PinA);
	self->PulseB = (uint16_t)((self->PortB->IDR & self->PinB) == self->PinB);

	self->curr = (self->PulseA << 1) | (self->PulseB);

	if(((self->curr ^ self->prev) != QEI_INVALID) && (self->curr != self->prev)){
		direction = (self->prev & QEI_PREV_MSK) ^ ((self->curr & QEI_CURR_MSK) >> 1);
		if(direction == 0) direction = -1;
		else __NOP();
		self->pulse -= direction;
	}
	self->prev = self->curr;
}

static void Encode_X2_QEI(QEI *self){
	self->PulseA = (uint16_t)((self->PortA->IDR & self->PinA) == self->PinA);
	self->PulseB = (uint16_t)((self->PortB->IDR & self->PinB) == self->PinB);
	self->curr = (self->PulseA << 1) | (self->PulseB);

	if(((self->prev == 0x3) && (self->curr == 0x0)) || ((self->prev == 0x0) && (self->curr == 0x3))){
		self->pulse++;
	}
	else if(((self->prev == 0x2) && (self->curr == 0x1)) || ((self->prev == 0x1) && (self->curr == 0x2))){
		self->pulse--;
	}
	self->prev = self->curr;
}

static void ResetQEI(QEI *self){
	self->pulse = 0;
}

static void GetRPM(QEI *self, uint32_t sample_t){
	self->start_t = HAL_GetTick();
	self->dt = self->start_t - self->prev_t;
	if(self->dt >= sample_t){
		int pulse_ = self->pulse;
		self->RPM = (pulse_ * 60000.0f) / (self->PPR * 4.0f * self->dt);
		pulse_ = 0;
		self->prev_t = self->start_t;
	}
}
static void GetRAD_S(QEI *self, uint32_t sample_t){
	self->start_t = HAL_GetTick();
	self->dt = self->start_t - self->prev_t;
	if(self->dt >= sample_t){
		int pulse_ = self->pulse;
		self->RAD_S = (pulse_ * M_TWOPI) / (self->PPR * 4.0f * self->dt);
		pulse_ = 0;
		self->prev_t = self->start_t;
	}
}
static void GetMTR_S(QEI *self, uint32_t sample_t, float r){
	self->start_t = HAL_GetTick();
	self->dt = self->start_t - self->prev_t;
	if(self->dt >= sample_t){
		int pulse_ = self->pulse;
		self->MTR_S = (pulse_ * M_TWOPI * r) / (self->PPR * 4.0f * self->dt);
		pulse_ = 0;
		self->prev_t = self->start_t;
	}
}

static void GetDEG(QEI *self){
	self->DEG = (self->pulse * 180.0f) / (4.0f * self->PPR);
}
static void GetRAD(QEI *self){
	self->RAD = (self->pulse * M_PI) / (4.0f * self->PPR);
}
static void GetMTR(QEI *self, float r){
	self->METER = (self->pulse * M_PI * r * 2.0f) / (4.0f * self->PPR);
}

