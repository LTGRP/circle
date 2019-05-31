/*
 * includes.h
 *
 *  Created on: 22 de dez de 2017
 *      Author: matheus
 */

#pragma once

#ifndef INCLUDES_H_
#define INCLUDES_H_

#include <Arduino.h>


#include <circle/startup.h>
#include <circle/logger.h>
#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <string.h>
#include <circle/timer.h>
#include <circle/gpiopin.h>
#include <circle/gpioclock.h>
#include <inttypes.h>
#include <circle/bcm2835.h>
#include <circle/memio.h>
#include <circle/synchronize.h>
#include <circle/input/touchscreen.h>
#include <circle/usb/dwhcidevice.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/cputhrottle.h>
#include <circle/screen.h>
#include <circle/bcmframebuffer.h>
#include <circle/dmachannel.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
#include <circle/usb/usbmouse.h>
#include <circle/input/touchscreen.h>
#include <inttypes.h>

extern "C" {
#include <lvgl/lvgl/lvgl.h>
#include <lvgl/lvgl/lv_themes/lv_theme_material.h>
}



#endif /* INCLUDES_H_ */
