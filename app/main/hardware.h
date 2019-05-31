/*
 * hardware.h
 *
 *  Created on: 22 de dez de 2017
 *      Author: matheus
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "includes.h"

namespace Hardware {
CMemorySystem Memory(TRUE);
CKernelOptions m_Options;
CSerialDevice m_Serial;
CDeviceNameService m_DeviceNameService;
CCPUThrottle systemClock(CPUSpeedMaximum);
CInterruptSystem m_Interrupt;
CTimer m_Timer(&m_Interrupt);
CLogger m_Logger(m_Options.GetLogLevel(), &m_Timer);
//CGPIOClock m_Clock0(GPIOClock0, GPIOClockSourceOscillator);
CGPIOPin PinCrank(4, GPIOModeOutput);
CGPIOPin PinCrank2(17, GPIOModeOutput);
CSecondaryProcessors Processors(&Memory);
CTouchScreenDevice m_TouchScreen;
CDWHCIDevice m_DWHCI(&m_Interrupt, &m_Timer);
CScreenDevice m_Screen(800, 480);
//CScreenDevice m_Screen(1280, 720);
CUSBMouseDevice *m_pMouseDevice;
CBcmFrameBuffer m_FrameBuffer(m_Options.GetWidth(), m_Options.GetHeight(), 16);
CDMAChannel m_ScreenDMA(DMA_CHANNEL_SCREEN);


void setupHardware() {
	systemClock.SetSpeed(CPUSpeedMaximum);
	m_Serial.Initialize(115200);

	CDevice *pTarget = m_DeviceNameService.GetDevice(
			Hardware::m_Options.GetLogDevice(),
			FALSE);

	m_Logger.Initialize(pTarget);
	m_Interrupt.Initialize();
	m_Timer.Initialize();
	m_Screen.Initialize();
	m_DWHCI.Initialize();
	m_FrameBuffer.Initialize();
	//m_TouchScreen.Initialize();
	Processors.Initialize();
}


void setupMouse(TMouseEventHandler *pEventHandler) {
	m_pMouseDevice = (CUSBMouseDevice *) CDeviceNameService::Get()->GetDevice(
			"umouse1", FALSE);
	if (m_pMouseDevice != 0) {
		if (m_pMouseDevice->Setup(Hardware::m_Screen.GetWidth(),
				m_Screen.GetHeight())) {
			m_pMouseDevice->ShowCursor(TRUE);
			m_pMouseDevice->RegisterEventHandler(pEventHandler);
		}
	}
}
}


#endif /* HARDWARE_H_ */
