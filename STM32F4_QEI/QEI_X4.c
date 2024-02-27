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

X4_QEI *Init_QEI(GPIO_TypeDef *PortA, GPIO_TypeDef *PortB, uint16_t PinA, uint16_t PinB, int32_t PPR){
	X4_QEI *self;
	self = (X4_QEI*)malloc(sizeof(X4_QEI));

	self->PortA = PortA;
	self->PortB = PortB;
	self->PinA = PinA;
	self->PinB = PinB;
	self->PPR = PPR;

	self->pulse = 0;
	self->PulseA = 0;
	self->PulseB = 0;
	self->curr = 0;
	self->prev = 0;

	self->GetPulse = EncodeQEI;
	self->ResetQEI = ResetQEI;

	self->RPM = self->RAD_S = self->MTR_S = 0;
	self->RAD = self->DEG = self->METER = 0;

	self->GetRAD_S = GetRAD_S;
	self->GetRPM = GetRPM;
	self->GetMTR_S = GetMTR_S;

	self->GetRAD = GetRAD;
	self->GetDEG = GetDEG;
	self->GetMTR = GetMTR;

	return self;
}

static void EncodeQEI(X4_QEI *self){
	int direction = 0; //1 = counter-clockwise, else clockwise
	self->PulseA = (uint32_t)((self->PortA->IDR & self->PinA) == self->PinA);
	self->PulseB = (uint32_t)((self->PortB->IDR & self->PinB) == self->PinB);

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
