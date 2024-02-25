/*
 * QEI_X4.h
 *
 *  Created on: Feb 25, 2024
 *      Author: Imaduddien
 */

#include "QEI_X4.h"

static void EncodeQEI(X4_QEI *self);
static void ResetQEI(X4_QEI *self);

static void GetRPM(X4_QEI *self, uint32_t sample_t);
static void GetRAD_S(X4_QEI *self, uint32_t sample_t);
static void GetMTR_S(X4_QEI *self, uint32_t sample_t, float r);

static void GetDEG(X4_QEI *self);
static void GetRAD(X4_QEI *self);
static void GetMTR(X4_QEI *self, float r);

X4_QEI *Init_QEI(GPIO_TypeDef *PortA, GPIO_TypeDef *PortB,
		uint16_t PinA, uint16_t PinB, int32_t PPR,
		X4_QEI_ROT Vel, X4_QEI_ANG Ang){
	X4_QEI *self;
	self = (X4_QEI*)malloc(sizeof(X4_QEI));

	self->PortA = PortA;
	self->PortB = PortB;
	self->PinA = PinA;
	self->PinB = PinB;
	self->PPR = PPR;

	self->pulse = self->PulseA = self->PulseB = 0;
	self->curr = self->prev = 0;
	self->start_t = self->prev_t = self->dt = 0;
	self->RPM = self->RAD_S = self->MTR_S = 0;
	self->RAD = self->DEG = self->METER = 0;
	
	self->GetPulse = EncodeQEI;
	self->ResetQEI = ResetQEI;

	if(Vel == QEI_RAD_S) self->GetROT_VEL = GetRAD_S;
	else self->GetROT_VEL = GetRPM;

	if(Ang == QEI_RAD) self->GetANGLE = GetRAD;
	else self->GetANGLE = GetDEG;

	self->GetLIN_VEL = GetMTR_S;
	self->GetDIST = GetMTR;
	return self;
}

static void EncodeQEI(X4_QEI *self){
	int direction = 0; //1 = counter-clockwise, else clockwise
	self->PulseA = (int16_t)HAL_GPIO_ReadPin(self->PortA, self->PinA);
	self->PulseB = (int16_t)HAL_GPIO_ReadPin(self->PortB, self->PinB);

	self->curr = (self->PulseA << 1) | (self->PulseB);

	if(((self->curr ^ self->prev) != QEI_INVALID) && (self->curr != self->prev)){
		direction = (self->prev & QEI_PREV_MSK) ^ ((self->curr & QEI_CURR_MSK) >> 1);
		if(direction == 0) direction = -1;
		self->pulse -= direction;
	}
	self->prev = self->curr;
}

static void ResetQEI(X4_QEI *self){
	self->pulse = 0;
}

static void GetRPM(X4_QEI *self, uint32_t sample_t){
	self->start_t = HAL_GetTick();
	self->dt = self->start_t - self->prev_t;
	if(self->dt >= sample_t){
		int pulse_ = self->pulse;
		self->RPM = (pulse_ * 60000.0f) / (self->PPR * 4.0f * self->dt);
		pulse_ = 0;
		self->prev_t = self->start_t;
	}
}
static void GetRAD_S(X4_QEI *self, uint32_t sample_t){
	self->start_t = HAL_GetTick();
	self->dt = self->start_t - self->prev_t;
	if(self->dt >= sample_t){
		int pulse_ = self->pulse;
		self->RAD_S = (pulse_ * M_TWOPI) / (self->PPR * 4.0f * self->dt);
		pulse_ = 0;
		self->prev_t = self->start_t;
	}
}
static void GetMTR_S(X4_QEI *self, uint32_t sample_t, float r){
	self->start_t = HAL_GetTick();
	self->dt = self->start_t - self->prev_t;
	if(self->dt >= sample_t){
		int pulse_ = self->pulse;
		self->MTR_S = (pulse_ * M_TWOPI * r) / (self->PPR * 4.0f * self->dt);
		pulse_ = 0;
		self->prev_t = self->start_t;
	}
}

static void GetDEG(X4_QEI *self){
	self->DEG = (self->pulse * 360.0f) / (4.0f * self->PPR);
}
static void GetRAD(X4_QEI *self){
	self->RAD = (self->pulse * M_TWOPI) / (4.0f * self->PPR);
}
static void GetMTR(X4_QEI *self, float r){
	self->METER = (self->pulse * M_TWOPI * r) / (4.0f * self->PPR);
}

