/*
 * displayDriver.h
 *
 *  Created on: 22 de dez de 2017
 *      Author: matheus
 */

#ifndef DISPLAYDRIVER_H_
#define DISPLAYDRIVER_H_

#include "includes.h"
#include "hardware.h"


typedef u32 TColor;

namespace DisplayDriver {

void ScreenFlushComplete(unsigned int var, boolean bStatus, void *param) {
	lv_flush_ready();
}

static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t * color_p) {
	/*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/
	/*
	int32_t act_x1 = x1 < 0 ? 0 : x1;
	int32_t act_y1 = y1 < 0 ? 0 : y1;
	int32_t act_x2 = x2 > 800 - 1 ? 800 - 1 : x2;
	int32_t act_y2 = y2 > 480 - 1 ? 480 - 1 : y2;
	uint32_t x;
	uint32_t y;
	for (y = act_y1; y <= act_y2; y++) {
		for (x = act_x1; x <= act_x2; x++) {

			Hardware::m_Screen.SetPixel(x, y, color_p->full);
			color_p++;
		}

		color_p += x2 - act_x2;
	}
	 */

	void *pDestination = (void *) (Hardware::m_FrameBuffer.GetBuffer()
			+ y1 * Hardware::m_FrameBuffer.GetPitch() + x1 * sizeof(lv_color_t));

	size_t nBlockLength = (x2 - x1 + 1) * sizeof(lv_color_t);

	Hardware::m_ScreenDMA.SetupMemCopy2D(pDestination, color_p,
			nBlockLength, y2 - y1 + 1,
			Hardware::m_FrameBuffer.GetPitch() - nBlockLength);


	//Hardware::m_ScreenDMA.SetCompletionRoutine(ScreenFlushComplete, NULL);
	Hardware::m_ScreenDMA.Start();
	Hardware::m_ScreenDMA.Wait();
	lv_flush_ready();

}
}


#endif /* DISPLAYDRIVER_H_ */
