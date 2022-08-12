#include "guiTopBar.h"

//Top bar GUI objects
lv_obj_t* topBarArea;
lv_obj_t* batteryLabel;
lv_obj_t* timeLabel;
lv_obj_t* powerModeLabel;
lv_obj_t* callSignLabel;

/**
  * @brief	This function initializes this GUI part
  * @param	None
  * @return	None
  */
void GUITopBarInit() {
	//Set Top Info Bar
	//Top Bar Area
	topBarArea = lv_obj_create(lv_scr_act());	//lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(topBarArea);
	lv_obj_add_style(topBarArea, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(topBarArea, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(topBarArea, 0, LV_PART_MAIN);
	lv_obj_set_size(topBarArea, 480, 25);
	lv_obj_align(topBarArea, LV_ALIGN_TOP_MID, 0, 0);
	//Battery Indicator
	batteryLabel = lv_label_create(topBarArea);
	lv_obj_remove_style_all(batteryLabel);
	lv_obj_add_style(batteryLabel, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(batteryLabel, 0, LV_PART_MAIN);
	lv_obj_set_size(batteryLabel, 25, 25);
//	lv_obj_set_pos(batteryLabel, LV_HOR_RES - 25, 0);
	lv_obj_align(batteryLabel, LV_ALIGN_RIGHT_MID, 0, 0);
	lv_obj_set_style_text_align(batteryLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(batteryLabel, LV_SYMBOL_BATTERY_3);
	//Time
	timeLabel = lv_label_create(topBarArea);
	lv_obj_remove_style_all(timeLabel);
	lv_obj_add_style(timeLabel, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(timeLabel, 0, LV_PART_MAIN);
	lv_obj_set_size(timeLabel, 60, 25);
//	lv_obj_set_pos(timeLabel, 0, 0);
	lv_obj_align(timeLabel, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_style_text_align(timeLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(timeLabel, "12:00");
	//Power Mode
//	powerModeLabel = lv_label_create(topBarArea);
//	lv_obj_remove_style_all(powerModeLabel);
//	lv_obj_add_style(powerModeLabel, &mainStyle, LV_PART_MAIN);
//	lv_obj_add_style(powerModeLabel, &mainStyleDisabled, LV_PART_MAIN);
//	lv_obj_set_style_pad_all(powerModeLabel, 0, LV_PART_MAIN);
//	lv_obj_set_style_pad_left(powerModeLabel, 2, LV_PART_MAIN);
//	lv_obj_set_size(powerModeLabel, 40, 20);
//	lv_obj_set_pos(powerModeLabel, 2, 2);
//	lv_label_set_text(powerModeLabel, "OFF");
	//Call Sign Label
	callSignLabel = lv_label_create(topBarArea);
	lv_obj_remove_style_all(callSignLabel);
	lv_obj_add_style(callSignLabel, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(callSignLabel, 0, LV_PART_MAIN);
//	lv_obj_set_style_bg_color(callSignLabel, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_size(callSignLabel, 120, 25);
//	lv_obj_set_pos(callSignLabel, 0, 0);
	lv_obj_align(callSignLabel, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_text_align(callSignLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(callSignLabel, "NBMRDO-1");
}
