#include "guiCF.h"

//Radio Configuration GUI objects
lv_obj_t* centerFrequencyArea;
lv_obj_t* centerFrequencyTextArea;
lv_obj_t* centerFrequencyLockLabel;
lv_obj_t* centerFrequencyBandLabel;
lv_obj_t* bandwidthTextArea;
lv_obj_t* afcRangeLabel;

/**
  * @brief	This function handles Text Area events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
char cfTextAreaPrevStr[20];
static void GUICFTextArea(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    const char * txt = lv_textarea_get_text(ta);
    if(code == LV_EVENT_FOCUSED) {
    	//Get current string
    	strcpy(cfTextAreaPrevStr, txt);
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(kb);
    }

    if(code == LV_EVENT_DEFOCUSED) {
    	//Cancel input value, recover old input
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        if(cfTextAreaPrevStr[0] != 0x00) {
        	strcpy(txt, cfTextAreaPrevStr);
        }
    }

    if(code == LV_EVENT_READY) {
    	//Keyboard OK symbol pressed, save and apply input value
    	lv_obj_clear_state(ta, LV_STATE_FOCUSED);
    	lv_keyboard_set_textarea(kb, NULL);
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
		cfTextAreaPrevStr[0] = 0x00;

		if(ta == centerFrequencyTextArea) {
			//Center Frequency Text Area Change apply
			uint32_t value;
			if(sscanf(txt, "%d", &value) == 1) {
				//Inform Radio Interface of value change
				InterThreadMessageStruct msg = {.id = InterThread_CenterFrequency, .data = (uint32_t*)value, .length = 0 };
				rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
			}
		}
		else if(ta == bandwidthTextArea) {
			//Bandwidth Text Area Change apply
			GUIRadioBandwidthUpdate();
			uint32_t value;
			if(sscanf(txt, "%d", &value) == 1) {
				//Inform Radio Interface of value change
				InterThreadMessageStruct msg = {.id = InterThread_Bandwidth, .data = (uint32_t*)value, .length = 0 };
				rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
			}
		}
    }

    if(code == LV_EVENT_VALUE_CHANGED) {
    	//Value changed
    }
}

/**
  * @brief	This function is used to update more complex GUI elements in this GUI part
  * @param	None
  * @return	None
  */
void GUIRadioBandwidthUpdate() {
	const char * str = lv_textarea_get_text(bandwidthTextArea);

	int32_t value;
	sscanf(str, "%d", &value);

	lv_obj_t* rfTrackLabel = lv_obj_get_child(rfTrackingArea, 2);

	char pt1[10];
	char pt2[10];
	char pt3[10];
	char pt4[10];
	sprintf(pt1, "%dk", -((value / 1000) >> 1));
	sprintf(pt2, "%dk", -((value / 1000) >> 2));
	sprintf(pt3, "%dk", ((value / 1000) >> 2));
	sprintf(pt4, "%dk", ((value / 1000) >> 1));
	char lblStr[100];
	sprintf(lblStr, "%-5s %-5s 0 %5s %5s", 	pt1, pt2, pt3, pt4);
	lv_label_set_text(rfTrackLabel, lblStr);
}

/**
  * @brief	This function initializes this GUI part
  * @param	None
  * @return	None
  */
void GUICFAreaInit() {
	//Center Frequency Indicator Label
	//Center Frequency Area
	centerFrequencyArea = lv_obj_create(lv_scr_act());	//lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(centerFrequencyArea);
	lv_obj_add_style(centerFrequencyArea, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(centerFrequencyArea, 0, LV_PART_MAIN);
	lv_obj_set_size(centerFrequencyArea, 272, 80);
	lv_obj_set_pos(centerFrequencyArea, 0, 25);

	//Center Frequency Text Area
	centerFrequencyTextArea = lv_textarea_create(centerFrequencyArea);	// lv_label_create(centerFrequencyArea);
	lv_obj_remove_style_all(centerFrequencyTextArea);
	lv_obj_add_style(centerFrequencyTextArea, &mainStyle, LV_PART_MAIN);
	lv_textarea_set_one_line(centerFrequencyTextArea, true);
	lv_textarea_set_max_length(centerFrequencyTextArea, 9);
	lv_obj_set_style_text_align(centerFrequencyTextArea, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
	lv_obj_set_style_border_width(centerFrequencyTextArea, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(centerFrequencyTextArea, 2, LV_PART_CURSOR | LV_STATE_FOCUSED);
	lv_obj_set_style_border_side(centerFrequencyTextArea, LV_BORDER_SIDE_LEFT, LV_PART_CURSOR);
	lv_obj_set_style_border_color(centerFrequencyTextArea, STYLE_COLOR_GREY, LV_PART_CURSOR);
	lv_obj_set_style_text_font(centerFrequencyTextArea, &lv_font_montserrat_40, LV_PART_MAIN);
	lv_obj_align(centerFrequencyTextArea, LV_ALIGN_TOP_RIGHT, 0, 0);
	lv_textarea_set_text(centerFrequencyTextArea, "436450000");
	lv_obj_add_event_cb(centerFrequencyTextArea, GUICFTextArea, LV_EVENT_FOCUSED, numPad);
	lv_obj_add_event_cb(centerFrequencyTextArea, GUICFTextArea, LV_EVENT_DEFOCUSED, numPad);
	lv_obj_add_event_cb(centerFrequencyTextArea, GUICFTextArea, LV_EVENT_READY, numPad);

	//Center Frequency Lock Label
	centerFrequencyLockLabel = lv_label_create(centerFrequencyArea);
	lv_obj_remove_style_all(centerFrequencyLockLabel);
	lv_obj_add_style(centerFrequencyLockLabel, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(centerFrequencyLockLabel, 0, LV_PART_MAIN);
//	lv_obj_set_style_border_color(centerFrequencyLockLabel, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_text_color(centerFrequencyLockLabel, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_align(centerFrequencyLockLabel, LV_ALIGN_BOTTOM_LEFT, 0, 0);
	lv_label_set_text(centerFrequencyLockLabel, "LOCK");

	//Channel Label
//	centerFrequencyBandLabel = lv_label_create(centerFrequencyArea);
//	lv_obj_remove_style_all(centerFrequencyBandLabel);
//	lv_obj_add_style(centerFrequencyBandLabel, &mainStyle, LV_PART_MAIN);
//	lv_obj_set_style_border_width(centerFrequencyBandLabel, 0, LV_PART_MAIN);
//	lv_obj_align(centerFrequencyBandLabel, LV_ALIGN_BOTTOM_MID, 0, 0);
//	lv_label_set_text(centerFrequencyBandLabel, "CH: 01");

	//Bandwidth Text Area
	bandwidthTextArea = lv_textarea_create(centerFrequencyArea);
	lv_obj_remove_style_all(bandwidthTextArea);
	lv_obj_add_style(bandwidthTextArea, &mainStyle, LV_PART_MAIN);
	lv_textarea_set_one_line(bandwidthTextArea, true);
	lv_textarea_set_max_length(bandwidthTextArea, 6);
	lv_obj_set_style_text_align(bandwidthTextArea, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
	lv_obj_set_style_pad_all(bandwidthTextArea, 2, LV_PART_MAIN);
	lv_obj_set_style_border_width(bandwidthTextArea, 2, LV_PART_CURSOR | LV_STATE_FOCUSED);
	lv_obj_set_style_border_side(bandwidthTextArea, LV_BORDER_SIDE_LEFT, LV_PART_CURSOR);
	lv_obj_set_style_border_color(bandwidthTextArea, STYLE_COLOR_GREY, LV_PART_CURSOR);
	lv_obj_set_style_text_font(bandwidthTextArea, &lv_font_montserrat_14, LV_PART_MAIN);
	lv_obj_set_size(bandwidthTextArea, 70, 25);
	lv_obj_align(bandwidthTextArea, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
	lv_textarea_set_text(bandwidthTextArea, "32000");
	lv_obj_add_event_cb(bandwidthTextArea, GUICFTextArea, LV_EVENT_FOCUSED, numPad);
	lv_obj_add_event_cb(bandwidthTextArea, GUICFTextArea, LV_EVENT_DEFOCUSED, numPad);
	lv_obj_add_event_cb(bandwidthTextArea, GUICFTextArea, LV_EVENT_READY, numPad);
	//Bandwidth Label
	lv_obj_t* label = lv_label_create(centerFrequencyArea);
	lv_obj_remove_style_all(label);
	lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(label, &mainStyleSelected, LV_PART_MAIN);
	lv_obj_set_style_pad_all(label, 2, LV_PART_MAIN);
	lv_obj_set_size(label, 40, 25);
	lv_obj_align(label, LV_ALIGN_BOTTOM_RIGHT, -65, 0);
	lv_label_set_text(label, "BW:");

//	//AFC Range Area
//	afcRangeLabel = lv_textarea_create(centerFrequencyArea);
//	lv_obj_remove_style_all(afcRangeLabel);
//	lv_obj_add_style(afcRangeLabel, &mainStyle, LV_PART_MAIN);
//	lv_textarea_set_one_line(afcRangeLabel, true);
//	lv_textarea_set_max_length(afcRangeLabel, 6);
//	lv_obj_set_style_text_align(afcRangeLabel, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
////	lv_obj_set_style_border_width(afcRangeLabel, 0, LV_PART_MAIN);
//	lv_obj_set_style_pad_all(afcRangeLabel, 2, LV_PART_MAIN);
//	lv_obj_set_style_border_width(afcRangeLabel, 2, LV_PART_CURSOR | LV_STATE_FOCUSED);
//	lv_obj_set_style_border_side(afcRangeLabel, LV_BORDER_SIDE_LEFT, LV_PART_CURSOR);
//	lv_obj_set_style_border_color(afcRangeLabel, STYLE_COLOR_GREY, LV_PART_CURSOR);
//	lv_obj_set_style_text_font(afcRangeLabel, &lv_font_montserrat_14, LV_PART_MAIN);
//	lv_obj_set_size(afcRangeLabel, 70, 25);
//	lv_obj_align(afcRangeLabel, LV_ALIGN_BOTTOM_MID, 0, 0);
//	lv_textarea_set_text(afcRangeLabel, "500000");
//	lv_obj_add_event_cb(afcRangeLabel, GUICFTextArea, LV_EVENT_FOCUSED, numPad);
//	lv_obj_add_event_cb(afcRangeLabel, GUICFTextArea, LV_EVENT_DEFOCUSED, numPad);
//	lv_obj_add_event_cb(afcRangeLabel, GUICFTextArea, LV_EVENT_READY, numPad);
//	//Bandwidth Label
//	lv_obj_t* label = lv_label_create(centerFrequencyArea);
//	lv_obj_remove_style_all(label);
//	lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
//	lv_obj_add_style(label, &mainStyleSelected, LV_PART_MAIN);
////	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
//	lv_obj_set_style_pad_all(label, 2, LV_PART_MAIN);
//	lv_obj_set_size(label, 40, 25);
//	lv_obj_align(label, LV_ALIGN_BOTTOM_MID, -32, 0);
//	lv_label_set_text(label, "BW:");
}
