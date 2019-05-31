/*
 * hmi_MainScreen.h
 *
 *  Created on: 22 de dez de 2017
 *      Author: matheus
 */

#ifndef HMI_MAINSCREEN_H_
#define HMI_MAINSCREEN_H_

#include "timer1.h"
#include "utils.h"
#include "hmi_DashboardScreen.h"
#include "hmi_simu.h"
#ifdef __cplusplus
extern "C" {
#include <lvgl/lvgl/lvgl.h>
#include <lvgl/lvgl/lv_themes/lv_theme_material.h>
}
#else
#include "lvgl/lvgl.h"
#endif

namespace Hmi_MainScreen {

void start(void) {

	static lv_style_t style_tv_btn_bg;
	lv_style_copy(&style_tv_btn_bg, &lv_style_plain);
	style_tv_btn_bg.body.main_color = LV_COLOR_HEX(0x487fb7);
	style_tv_btn_bg.body.grad_color = LV_COLOR_HEX(0x487fb7);
	style_tv_btn_bg.body.padding.ver = 0;

	static lv_style_t style_tv_btn_rel;
	lv_style_copy(&style_tv_btn_rel, &lv_style_btn_rel);
	style_tv_btn_rel.body.empty = 1;
	style_tv_btn_rel.body.border.width = 0;

	static lv_style_t style_tv_btn_pr;
	lv_style_copy(&style_tv_btn_pr, &lv_style_btn_pr);
	style_tv_btn_pr.body.radius = 0;
	style_tv_btn_pr.body.opa = LV_OPA_50;
	style_tv_btn_pr.body.main_color = LV_COLOR_WHITE;
	style_tv_btn_pr.body.grad_color = LV_COLOR_WHITE;
	style_tv_btn_pr.body.border.width = 0;
	style_tv_btn_pr.text.color = LV_COLOR_GRAY;

	lv_obj_t *tv = lv_tabview_create(lv_scr_act(), NULL);

	lv_obj_t *tab1 = lv_tabview_add_tab(tv, "Dashboard");
	lv_obj_t *tab2 = lv_tabview_add_tab(tv, "Simulation");
	lv_obj_t *tab3 = lv_tabview_add_tab(tv, "Manual");
	lv_obj_t *tab4 = lv_tabview_add_tab(tv, "Advanced");

	/*Blue bg instead of wallpaper*/
	lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BG, &style_tv_btn_bg);

	lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_BG, &style_tv_btn_bg);
	lv_tabview_set_style(tv, LV_TABVIEW_STYLE_INDIC, &lv_style_plain);
	lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_REL, &style_tv_btn_rel);
	lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_PR, &style_tv_btn_pr);
	lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_TGL_REL, &style_tv_btn_rel);
	lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_TGL_PR, &style_tv_btn_pr);

	Hmi_DashboardScreen::create(tab4);
	Hmi_SimuScreen::create(tab2);
}


}

#endif /* HMI_MAINSCREEN_H_ */
