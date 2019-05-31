/*
 * includes.h
 *
 *  Created on: 22 de dez de 2017
 *      Author: matheus
 */

#ifndef HMI_H_
#define HMI_H_

#include "includes.h"
#include "hmi_MainScreen.h"
#include "displayDriver.h"
#include "hardware.h"

namespace HMI {
static bool ex_tp_read(lv_indev_data_t *data);
void MouseEvent(TMouseEvent Event, unsigned nButtons, unsigned nPosX,
		unsigned nPosY);
void TimerHandler(unsigned hTimer, void *pParam, void *pContext);

uint8_t mouseClicked = 0;
int mouseY;
int mouseX;

void setupHMI() {

	lv_init();
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.disp_flush = DisplayDriver::ex_disp_flush;
	lv_disp_drv_register(&disp_drv);

	lv_indev_drv_t indev_drv; /*Descriptor of an input device driver*/
	lv_indev_drv_init(&indev_drv); /*Basic initialization*/
	indev_drv.type = LV_INDEV_TYPE_POINTER; /*The touchpad is pointer type device*/
	indev_drv.read = ex_tp_read; /*Library ready your touchpad via this function*/
	lv_indev_drv_register(&indev_drv); /*Finally register the driver*/

	lv_theme_t *th = lv_theme_default_init(210, NULL);
	lv_theme_set_current(th);

	Hardware::setupMouse(MouseEvent);
	Hardware::m_Timer.StartKernelTimer(10, TimerHandler);
}

void startHMI() {
	Hmi_MainScreen::start();
}

static bool ex_tp_read(lv_indev_data_t *data) {

	data->state = mouseClicked ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
	data->point.x = mouseX;
	data->point.y = mouseY;
	return false; /*false: no more data to read because we are no buffering*/
}

void MouseEvent(TMouseEvent Event, unsigned nButtons, unsigned nPosX,
		unsigned nPosY) {
	mouseX = nPosX;
	mouseY = nPosY;

	switch (Event) {
	case MouseEventMouseDown:
		if (nButtons & MOUSE_BUTTON_LEFT) {
			mouseClicked = 1;
		}
		break;

	case MouseEventMouseUp:
		if (nButtons & MOUSE_BUTTON_LEFT) {
			mouseClicked = 0;
		}
		break;

	default:
		break;
	}

}

void TimerHandler(unsigned hTimer, void *pParam, void *pContext) {
	Hardware::m_pMouseDevice->UpdateCursor();
	lv_tick_inc(10);
	Hardware::m_Timer.StartKernelTimer(10, TimerHandler);
}

inline void update() {
	lv_task_handler();
}

}

#endif /* INCLUDES_H_ */
