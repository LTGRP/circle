#pragma once

#ifndef Arduino_h
#define Arduino_h

#include "kernel.h"
#include <circle/timer.h>
#include <circle/serial.h>
#include <circle/memory.h>
#include <circle/logger.h>
#include <circle/startup.h>
#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/screen.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/gpioclock.h>
#include <circle/gpiopin.h>
#include <circle/pwmoutput.h>
#include <assert.h>
#include <circle/synchronize.h>
#include <circle/types.h>





//typedef u8  uint8_t;
//typedef u16 uint16_t;
//typedef u32 uint32_t;

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define PROGMEM
#define pgm_read_byte(addr)             (*(const uint8_t *) (addr))

#define noInterrupts()                  EnterCritical ()
#define interrupts()                    LeaveCritical ()
#define micros()                        CTimer::Get ()->GetClockTicks()
#define millis()                        CTimer::Get ()->GetTicks()
#define delayMicroseconds(usecs)        CTimer::Get ()->usDelay (usecs)
#define delay(msecs)                    CTimer::Get ()->MsDelay (msecs)

#define DIRECT_READ(base, mask)         (m_Pin.Read () ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   m_Pin.SetMode (GPIOModeInput, FALSE)
#define DIRECT_MODE_OUTPUT(base, mask)  m_Pin.SetMode (GPIOModeOutput, FALSE)
#define DIRECT_WRITE_LOW(base, mask)    m_Pin.Write (LOW)
#define DIRECT_WRITE_HIGH(base, mask)   m_Pin.Write (HIGH)

#define String CString



void setup();
int loop();
void loop2() __attribute__ ((weak));
void loop3() __attribute__ ((weak));
void loop4() __attribute__ ((weak));




class CSecondaryProcessors:
public CMultiCoreSupport
{
public:
	CSecondaryProcessors(CMemorySystem *pMemorySystem) :
    CMultiCoreSupport (pMemorySystem)
    {
    }
	~CSecondaryProcessors(void){};



	void Run (unsigned nCore){
{
	switch (nCore)
	{
	case 1:
        while(1){
		loop2();
        }
		break;

	case 2:
		while(1){
		loop3();
        }
		break;

	case 3:
		while(1){
		loop4();
        }
		break;
	}
}
}


};

float sqrt(const float x);

#endif
