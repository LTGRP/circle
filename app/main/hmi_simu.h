/*
 * hmi_simu.h
 *
 *  Created on: 27 de dez de 2017
 *      Author: matheus
 */

#ifndef HMI_SIMU_H_
#define HMI_SIMU_H_
#ifdef __cplusplus
extern "C" {
#include <lvgl/lvgl/lvgl.h>
#include <lvgl/lvgl/lv_themes/lv_theme_material.h>
}
#else
#include "lvgl/lvgl.h"
#endif

namespace Hmi_SimuScreen {

static lv_obj_t *chart;
static lv_chart_series_t *dl1;
lv_obj_t * gaugeRPM;
uint8_t rpmok = 0;

void addPoint(uint16_t value) {
	lv_chart_set_next(chart, dl1, value);
	lv_chart_refresh(chart);
}

void create(lv_obj_t *parent) {

	chart = lv_chart_create(parent, NULL); /*Create the chart*/
	lv_obj_set_size(chart, lv_obj_get_width(parent) / 2,
			lv_obj_get_width(parent) / 3); /*Set the size*/
	lv_obj_align(chart, parent, LV_ALIGN_OUT_TOP_RIGHT, 0, 150); /*Align below the slider*/
	lv_chart_set_series_width(chart, 2); /*Set the line width*/


	/*Add a RED data series and set some points*/
	dl1 = lv_chart_add_series(chart, LV_COLOR_RED);
	lv_chart_set_point_count(chart, 100);
	lv_chart_set_div_line_count(chart, 10, 6);
	lv_chart_set_range(chart, 0, 6000);

	static lv_style_t style;
	lv_style_copy(&style, &lv_style_pretty_color);
	style.body.main_color = LV_COLOR_HEX3(0x666); /*Line color at the beginning*/
	style.body.grad_color = LV_COLOR_HEX3(0x666); /*Line color at the end*/
	style.body.padding.hor = 10; /*Scale line length*/
	style.body.padding.inner = 8; /*Scale label padding*/
	style.body.border.color = LV_COLOR_HEX3(0x333); /*Needle middle circle color*/
	style.line.width = 3;
	style.text.color = LV_COLOR_HEX3(0x333);
	style.line.color = LV_COLOR_RED; /*Line color after the critical value*/

	/*Describe the color for the needles*/
	static lv_color_t needle_colors[] = { LV_COLOR_BLUE, LV_COLOR_ORANGE,
			LV_COLOR_PURPLE };

	/*Create a gauge*/
	gaugeRPM = lv_gauge_create(parent, NULL);
	lv_gauge_set_style(gaugeRPM, &style);
	lv_gauge_set_needle_count(gaugeRPM, 1, needle_colors);
	lv_obj_align(gaugeRPM, parent, LV_ALIGN_IN_TOP_LEFT, 20, 50);
	lv_obj_set_size(gaugeRPM, lv_obj_get_width(parent) / 3,
			lv_obj_get_width(parent) / 3); /*Set the size*/
	/*Set the values*/
	lv_gauge_set_range(gaugeRPM, 0, 6000);
	lv_gauge_set_critical_value(gaugeRPM, 5000);
	lv_gauge_set_scale(gaugeRPM, 220, 13, 7);
	lv_gauge_set_value(gaugeRPM, 0, 10);
	rpmok = 1;

}

}
#endif /* HMI_SIMU_H_ */
