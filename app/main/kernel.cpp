//
// kernel.cpp
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2016  R. Stange <rsta2@o2online.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "includes.h"
#include "hardware.h"
#include "timer1.h"
#include "hmi.h"
#include "Engine0.h"
#include "rtwtypes.h"
#include "zero_crossing_types.h"

#define MODEL_TIME 0

void rt_OneStep(void) {
	static boolean_T OverrunFlag = false;

	// Disable interrupts here

	// Check for overrun
	if (OverrunFlag) {
		rtmSetErrorStatus(rtM, "Overrun");
		return;
	}

	OverrunFlag = true;

	// Save FPU context here (if necessary)
	// Re-enable timer or interrupt here
	// Set model inputs here

	// Step the model for base rate
	//Engine_step();
	Engine0_step();
	// Get model outputs here

	// Indicate task complete
	OverrunFlag = false;

	// Disable interrupts here
	// Restore FPU context here (if necessary)
	// Enable interrupts here
}

CString text;

void setup() {
	Engine0_initialize();
	rtU.ThrottleAngle = 8.973;
	rtU.SparkAdvanceBTDC = 15.0;
	rtU.Load = 25.0;
	Hardware::setupHardware();
	Hardware::systemClock.SetSpeed(CPUSpeedMaximum);
	Timer1::setupTimer1();

	Hardware::m_Logger.Write("Kernel", LogNotice,
			"Compile time: " __DATE__ " " __TIME__);
	text.Format("Available total memory: %dMB\n",
			Hardware::Memory.GetMemSize() / 1000000UL);
	text.Format("CPUCLOCK: %d", Hardware::systemClock.GetClockRate());
	Hardware::m_Logger.Write("Kernel", LogNotice, text);
	text.Format("Temperature: %dºC", Hardware::systemClock.GetTemperature());
	Hardware::m_Logger.Write("Kernel", LogNotice, text);
	text.Format("Screen Height: %d, Width: %d\n",
			Hardware::m_Screen.GetHeight(), Hardware::m_Screen.GetWidth());
	Hardware::m_Logger.Write("Kernel", LogNotice, text);

	HMI::setupHMI();
	HMI::startHMI();


}


int loop() {
	HMI::update();
	delay(1);
	return 0;
}

CString core3text;

void loop2() {
#if MODEL_TIME==1
	uint32_t time1, time2;
	time1 = micros();
	rt_OneStep();
	time2 = micros();
	core3text.Format("Time(us): %d Freq=%d\n", time2 - time1,
			Hardware::systemClock.GetClockRate());
	Hardware::m_Serial.Write(core3text, core3text.GetLength());
	delay(100);
#else
	rt_OneStep();
	delayMicroseconds(100);
#endif


}



void loop3() {
#if MODEL_TIME==0
	//core3text.Format("%.2f\n", rtY.RPM);
	//Hardware::m_Serial.Write(core3text, core3text.GetLength());
		Hmi_SimuScreen::addPoint((uint16_t) rtY.RPM);
	if (Hmi_SimuScreen::rpmok)
		lv_gauge_set_value(Hmi_SimuScreen::gaugeRPM, 0, (uint16_t) rtY.RPM);
	Timer1::Timer1SetFrequency((uint16_t) rtY.RPM);
	Timer1::Timer1SetDuty(50);
	delay(50);
#endif
}

void loop4() {
	delay(5000);
	rtU.ThrottleAngle = 11.93;
	delay(1000);
	rtU.ThrottleAngle = 5.0;
	delay(3000);
	rtU.ThrottleAngle = 25.0;
	delay(3000);
	rtU.ThrottleAngle = 8.93;
}


