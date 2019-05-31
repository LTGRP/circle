/*
 * hmi_DashboardScreen.h
 *
 *  Created on: 22 de dez de 2017
 *      Author: matheus
 */

#ifndef HMI_DASHBOARDSCREEN_H_
#define HMI_DASHBOARDSCREEN_H_

#include "includes.h"
#include "hmi_MainScreen.h"
extern "C" {
#include <lvgl/lvgl/lvgl.h>
#include <lvgl/lvgl/lv_themes/lv_theme_material.h>
}

namespace Hmi_DashboardScreen {
static lv_res_t btn_shutdown(lv_obj_t *btn);

static lv_res_t list_btn_action(lv_obj_t *slider);

void create(lv_obj_t *parent) {
	lv_page_set_style(parent, LV_PAGE_STYLE_BG, &lv_style_transp_fit);
	lv_page_set_style(parent, LV_PAGE_STYLE_SCRL, &lv_style_transp_fit);

	lv_page_set_scrl_fit(parent, false, false);
	lv_page_set_scrl_height(parent, lv_obj_get_height(parent));
	lv_page_set_sb_mode(parent, LV_SB_MODE_OFF);

	/*Create styles for the buttons*/
	static lv_style_t style_btn_rel;
	static lv_style_t style_btn_pr;
	lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
	style_btn_rel.body.main_color = LV_COLOR_HEX3(0x333);
	style_btn_rel.body.grad_color = LV_COLOR_BLACK;
	style_btn_rel.body.border.color = LV_COLOR_SILVER;
	style_btn_rel.body.border.width = 1;
	style_btn_rel.body.border.opa = LV_OPA_50;
	style_btn_rel.body.radius = 0;

	lv_style_copy(&style_btn_pr, &style_btn_rel);
	style_btn_pr.body.main_color = LV_COLOR_MAKE(0x55, 0x96, 0xd8);
	style_btn_pr.body.grad_color = LV_COLOR_MAKE(0x37, 0x62, 0x90);
	style_btn_pr.text.color = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);

	lv_obj_t *list = lv_list_create(parent, NULL);
	//lv_obj_set_height(list, 2 * lv_obj_get_height(parent) / 3);
	lv_obj_set_height(list, lv_obj_get_height(parent));
	lv_list_set_style(list, LV_LIST_STYLE_BG, &lv_style_transp_tight);
	lv_list_set_style(list, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
	lv_list_set_style(list, LV_LIST_STYLE_BTN_REL, &style_btn_rel);
	lv_list_set_style(list, LV_LIST_STYLE_BTN_PR, &style_btn_pr);
	lv_obj_align(list, NULL, LV_ALIGN_IN_LEFT_MID, 0, LV_DPI / 4);

	lv_list_add(list, SYMBOL_FILE, "New", list_btn_action);
	lv_list_add(list, SYMBOL_DIRECTORY, "Open", list_btn_action);
	lv_list_add(list, SYMBOL_TRASH, "Delete", list_btn_action);
	lv_list_add(list, SYMBOL_EDIT, "Edit", list_btn_action);
	lv_list_add(list, SYMBOL_SAVE, "Save", list_btn_action);
	lv_list_add(list, SYMBOL_WIFI, "WiFi", list_btn_action);
	lv_list_add(list, SYMBOL_POWER, "Power", btn_shutdown);

	lv_obj_t *mbox = lv_mbox_create(parent, NULL);
	lv_mbox_set_text(mbox, "Click a button to copy its text to the Text area ");
	lv_obj_set_width(mbox, LV_HOR_RES - LV_DPI);
	static const char * mbox_btns[] = { "Got it", "" };
	lv_mbox_add_btns(mbox, mbox_btns, NULL); /*The default action is close*/
	lv_obj_align(mbox, parent, LV_ALIGN_IN_TOP_MID, 0, LV_DPI / 2);
}

static lv_res_t list_btn_action(lv_obj_t *btn) {

	return LV_RES_OK;
}

static lv_res_t btn_shutdown(lv_obj_t *btn) {
	reboot();
	return LV_RES_OK;
}

}

#endif /* HMI_DASHBOARDSCREEN_H_ */
