#include "guiTracking.h"

//Radio Tracking Information GUI objects
lv_obj_t* rssiTrackingArea;
lv_obj_t* rssiTrackingBar;
lv_obj_t* rfTrackingArea;
lv_obj_t* rfTrackingBar;
lv_obj_t* rfAFCRangeBar;
lv_obj_t* emptyTrackingArea;

/**
  * @brief	This function initializes this GUI part
  * @param	None
  * @return	None
  */
void GUITrackingAreaInit() {
	//Set Tracking Bars
	//RSSI Indicator Area
	rssiTrackingArea = lv_obj_create(lv_scr_act());	//lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(rssiTrackingArea);
	lv_obj_add_style(rssiTrackingArea, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(rssiTrackingArea, 0, LV_PART_MAIN);
	lv_obj_set_size(rssiTrackingArea, 210, 28);
	lv_obj_set_pos(rssiTrackingArea, 270, 25);
	//RSSI Indicator Bar
	rssiTrackingBar = lv_bar_create(rssiTrackingArea);
	lv_obj_remove_style_all(rssiTrackingBar);
	lv_obj_add_style(rssiTrackingBar, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(rssiTrackingBar, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(rssiTrackingBar, 0, LV_PART_MAIN);
	lv_obj_set_style_bg_color(rssiTrackingBar, lv_color_hex(0xFF0000), LV_PART_INDICATOR);
	lv_obj_set_style_bg_grad_color(rssiTrackingBar, lv_color_hex(0x00FF00), LV_PART_INDICATOR);
	lv_obj_set_style_bg_grad_dir(rssiTrackingBar, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);
	lv_obj_set_style_bg_opa(rssiTrackingBar, LV_OPA_100, LV_PART_INDICATOR);
	lv_obj_set_size(rssiTrackingBar, 210 - 16*2, 5);
	lv_obj_set_pos(rssiTrackingBar, 17, 4);
	lv_bar_set_range(rssiTrackingBar, 0, 128);
	lv_bar_set_value(rssiTrackingBar, 15, LV_ANIM_OFF);
	//RSSI Indicator Scale
	CustomScaleCreateParent(rssiTrackingArea, 16, 18, 2, 7, 3, false);
	CustomScaleCreateParent(rssiTrackingArea, 16, 18, 1, 5, 11, false);
	//RSSI Indicator Scale Label
	lv_obj_t* label = lv_label_create(rssiTrackingArea);
	lv_obj_remove_style_all(label);
	lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(label, 0, LV_PART_MAIN);
	lv_obj_set_style_text_font(label, &lv_font_unscii_8, LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -2);
	lv_label_set_text(label, "-128  -96  -64   -32    0");

	//Set RF Offset Tracking Indicator
	//RF Offset Tracking Indicator Area
	rfTrackingArea = lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(rfTrackingArea);
	lv_obj_add_style(rfTrackingArea, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(rfTrackingArea, 0, LV_PART_MAIN);
	lv_obj_set_size(rfTrackingArea, 210, 28);
	lv_obj_set_pos(rfTrackingArea, 270, 25+26);
	//AFC Range Bar
	rfAFCRangeBar = lv_line_create(rfTrackingArea);
	static lv_point_t rfTrackingBandwidthPoints[] = { {0, 0}, {90, 0} };
	lv_line_set_points(rfAFCRangeBar, rfTrackingBandwidthPoints, 2);
	lv_obj_remove_style_all(rfAFCRangeBar);
	lv_obj_add_style(rfAFCRangeBar, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(rfAFCRangeBar, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(rfAFCRangeBar, 0, LV_PART_MAIN);
	lv_obj_set_style_line_color(rfAFCRangeBar, lv_color_hex(0xFF0000), LV_PART_MAIN);
	lv_obj_set_style_line_width(rfAFCRangeBar, 4, LV_PART_MAIN);
	lv_obj_set_pos(rfAFCRangeBar, (210 - 90) / 2, 7);
	//Scale
	CustomScaleCreateParent(rfTrackingArea, 16, 18, 2, 7, 3, false);
	//Scale Label
	label = lv_label_create(rfTrackingArea);
	lv_obj_remove_style_all(label);
	lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(label, 0, LV_PART_MAIN);
	lv_obj_set_style_text_font(label, &lv_font_unscii_8, LV_PART_MAIN);
	lv_obj_set_style_max_width(label, 206, LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -2);
	lv_label_set_text(label, "-16k   -8k  0    8k   16k");
	//RF Offset Indicator Bar
	static lv_point_t rfTrackingIndicatorPoints[] = { {0, 0}, {0, 7} };
	rfTrackingBar = lv_line_create(rfTrackingArea);
	lv_line_set_points(rfTrackingBar, rfTrackingIndicatorPoints, 2);
	lv_obj_remove_style_all(rfTrackingBar);
	lv_obj_add_style(rfTrackingBar, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(rfTrackingBar, LV_OPA_0, LV_PART_MAIN);
	lv_obj_set_style_border_width(rfTrackingBar, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(rfTrackingBar, 0, LV_PART_MAIN);
	lv_obj_set_style_line_color(rfTrackingBar, lv_color_hex(0x00FF00), LV_PART_MAIN);
	lv_obj_set_style_line_width(rfTrackingBar, 3, LV_PART_MAIN);
	lv_obj_set_pos(rfTrackingBar, 50, 2);

	//Set Empty Tracking Indicator
	//Empty Tracking Indicator Area
	emptyTrackingArea = lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(emptyTrackingArea);
	lv_obj_add_style(emptyTrackingArea, &mainStyle, LV_PART_MAIN);
//	lv_obj_set_style_border_width(rssiTrackingArea, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(emptyTrackingArea, 0, LV_PART_MAIN);
	lv_obj_set_size(emptyTrackingArea, 210, 28);
	lv_obj_set_pos(emptyTrackingArea, 270, 25+26+26);
}
