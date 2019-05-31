/*
 * timer1.h
 *
 *  Created on: 22 de dez de 2017
 *      Author: matheus
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include "includes.h"
#include "hardware.h"
#include "utils.h"

namespace Timer1 {

#define CLOCK_DIVIDER 400
#define TOOTHS 60
#define TOOTHS_MISSING 2

uint_fast8_t count = 500;
uint_fast8_t high = 100;

#define BASE_FREQ 1000000UL

void Timer1SetFrequency(uint32_t frequency) {
	count = BASE_FREQ / frequency;
}

void Timer1SetDuty(uint8_t duty) {
	high = Utils::map(duty, 0, 100, 10, count - 10);
}

inline void Timer1Rearm(uint_fast8_t time) {
	u32 nCompare = read32(ARM_SYSTIMER_C1) + time;
	write32(ARM_SYSTIMER_C1, nCompare);
	if (nCompare < read32(ARM_SYSTIMER_CLO))		// time may drift
			{
		nCompare = read32(ARM_SYSTIMER_CLO) + time;
		write32(ARM_SYSTIMER_C1, nCompare);
	}
}

void Interrupt(void *pParam) {
	static uint8_t stepCounter = 0;
	PeripheralEntry();
	write32(ARM_SYSTIMER_CS, 1 << 1);
	PeripheralExit();

	if (Hardware::PinCrank.Read() == HIGH) {
		if (stepCounter <= TOOTHS - TOOTHS_MISSING) {
			++stepCounter;
			Hardware::PinCrank.Write(LOW);
			Hardware::PinCrank2.Write(LOW);
			Timer1Rearm(high);
		} else if (stepCounter <= TOOTHS) {
			stepCounter = 0;
			Timer1Rearm(count);
			Hardware::PinCrank2.Write(HIGH);
		} else {
			Hardware::PinCrank2.Write(LOW);
			Timer1Rearm(count);
			++stepCounter;
		}

	} else {
		Hardware::PinCrank.Write(HIGH);
		Timer1Rearm(count - high);
		Hardware::PinCrank2.Write(LOW);
	}

}

void setupTimer1() {
	Hardware::m_Interrupt.ConnectFIQ(ARM_FIQ_TIMER1, Interrupt, NULL);

	PeripheralEntry();
	write32(ARM_SYSTIMER_CLO, -(30 * CLOCKHZ));	// timer wraps soon, to check for problems
	write32(ARM_SYSTIMER_C1, read32(ARM_SYSTIMER_CLO) + count);
	PeripheralExit();
}

}



#endif /* TIMER1_H_ */
