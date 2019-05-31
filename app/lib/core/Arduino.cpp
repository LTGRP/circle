#include <Arduino.h>
#include "kernel.h"
#include <circle/multicore.h>
#include <circle/memory.h>
#include <circle/screen.h>
#include <circle/types.h>



CKernel::CKernel (void)
{
}

CKernel::~CKernel (void)
{
}

boolean CKernel::Initialize (void)
{
    
    setup();
    
    return TRUE;
    
}

TShutdownMode CKernel::Run (void)
{
    while (loop()!=-1);
    
    return ShutdownReboot;
}

float sqrt(const float x) {
	union {
		int i;
		float x;
	} u;
	u.x = x;
	u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
	u.x = u.x + x / u.x;
	u.x = 0.25f * u.x + x / u.x;

	return u.x;
}








