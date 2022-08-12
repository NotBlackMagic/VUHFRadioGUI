#include "guiBasicRadioConfig.h"

//Radio Basic Configuration GUI Objects
lv_obj_t* radioModeDropDown;
lv_obj_t* radioModulationDropDown;
lv_obj_t* radioAGCDropDown;
lv_obj_t* radioAFCDropDown;
lv_obj_t* radioAFCEnableButton;
lv_obj_t* radioTNCEnableButton;

/**
  * @brief	This function handles button events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
static void GUIBasicRadioConfigButtonEvent(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t * btn = lv_event_get_target(e);
	if(code == LV_EVENT_VALUE_CHANGED) {
		if(btn == radioAFCEnableButton) {
			uint32_t value;
			if(lv_obj_has_state(btn, LV_STATE_CHECKED) == true) {
				//Enable AFC
				lv_obj_clear_flag(rfAFCRangeBar, LV_OBJ_FLAG_HIDDEN);
				value = 0x01;
			}
			else {
				//Disable AFC
				lv_obj_add_flag(rfAFCRangeBar, LV_OBJ_FLAG_HIDDEN);
				value = 0x00;
			}
			//Inform Radio Interface of value change
			InterThreadMessageStruct msg = {.id = InterThread_AFC, .data = (uint32_t*)value, .length = 0 };
			rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
		}
		else if(btn == radioTNCEnableButton) {
			uint32_t value;
			if(lv_obj_has_state(btn, LV_STATE_CHECKED) == true) {
				//Enable TNC Mode
				value = 0x01;
			}
			else {
				//Disable TNC Mode
				value = 0x00;
			}
			//Inform Radio Interface of value change
			InterThreadMessageStruct msg = {.id = InterThread_TNCEnable, .data = (uint32_t*)value, .length = 0 };
			rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
		}
	}
}

/**
  * @brief	This function handles Drop Down events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
static void GUIBasicRadioConfigDropDownEvent(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t * obj = lv_event_get_target(e);
	if(code == LV_EVENT_VALUE_CHANGED) {
		if(obj == radioModeDropDown) {
			uint16_t selected = lv_dropdown_get_selected(obj);
			if(selected == 0) {
				lv_obj_set_style_border_color(obj, STYLE_COLOR_DARK_RED, LV_PART_MAIN);
				lv_obj_set_style_text_color(obj, STYLE_COLOR_RED, LV_PART_MAIN);
			}
			else {
				lv_obj_set_style_border_color(obj, STYLE_COLOR_DARK_GREEN, LV_PART_MAIN);
				lv_obj_set_style_text_color(obj, STYLE_COLOR_GREEN, LV_PART_MAIN);
			}
			//Inform Radio Interface of value change
			InterThreadMessageStruct msg = {.id = InterThread_Mode, .data = (uint32_t*)selected, .length = 0 };
			rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
		}
		else if(obj == radioModulationDropDown) {
			uint16_t selected = lv_dropdown_get_selected(obj);
			//Inform Radio Interface of value change
			InterThreadMessageStruct msg = {.id = InterThread_Modulation, .data = (uint32_t*)selected, .length = 0 };
			rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
		}
		else if(obj == radioAFCDropDown) {
			uint16_t selected = lv_dropdown_get_selected(obj);
			//Inform Radio Interface of value change
			InterThreadMessageStruct msg = {.id = InterThread_AGCSpeed, .data = (uint32_t*)selected, .length = 0 };
			rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
		}
	}
}

/**
  * @brief	This function initializes this GUI part
  * @param	None
  * @return	None
  */
void GUIBasicRadioConfigAreaInit() {
	//Radio Mode Drop Down
	radioModeDropDown = lv_dropdown_create(lv_scr_act());
	lv_dropdown_set_options(radioModeDropDown,	"OFF\n"
												"RX\n"
												"TX");
	lv_obj_remove_style_all(radioModeDropDown);
	lv_obj_add_style(radioModeDropDown, &mainStyle, 0);
	lv_dropdown_set_symbol(radioModeDropDown, NULL);
	lv_obj_set_size(radioModeDropDown, 60, 30);
	lv_obj_set_pos(radioModeDropDown, 0, 105);
	//Style Drop Down list items
	lv_obj_t* list = lv_dropdown_get_list(radioModeDropDown);
	lv_obj_remove_style_all(list);
	lv_obj_add_style(list, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(list, &mainStyleSelected, LV_PART_SELECTED);
	lv_obj_set_style_text_line_space(list, 6, LV_PART_MAIN);
	//Add event
	lv_obj_add_event_cb(radioModeDropDown, GUIBasicRadioConfigDropDownEvent, LV_EVENT_VALUE_CHANGED, NULL);

	//Radio Modulation Drop Down
	radioModulationDropDown = lv_dropdown_create(lv_scr_act());
	lv_dropdown_set_options(radioModulationDropDown,	"OFF\n"
														"AM\n"
														"FM\n"
														"AFSK\n"
														"ASK\n"
														"FSK\n"
														"GMSK\n"
														"4FSK\n"
														"BPSK\n"
														"QPSK");
	lv_obj_remove_style_all(radioModulationDropDown);
	lv_obj_add_style(radioModulationDropDown, &mainStyle, 0);
	lv_dropdown_set_symbol(radioModulationDropDown, NULL);
	lv_obj_set_size(radioModulationDropDown, 60, 30);
	lv_obj_set_pos(radioModulationDropDown, 60, 105);
	//Style Drop Down list items
	list = lv_dropdown_get_list(radioModulationDropDown);
	lv_obj_remove_style_all(list);
	lv_obj_add_style(list, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(list, &mainStyleSelected, LV_PART_SELECTED);
	lv_obj_set_style_text_line_space(list, 6, LV_PART_MAIN);
	//Add event
	lv_obj_add_event_cb(radioModulationDropDown, GUIBasicRadioConfigDropDownEvent, LV_EVENT_VALUE_CHANGED, NULL);

	//Radio AGC Drop Down
	radioAGCDropDown = lv_dropdown_create(lv_scr_act());
	lv_dropdown_set_options(radioAGCDropDown,	"AGC FAST\n"
												"AGC M-FS\n"
												"AGC MED\n"
												"AGC M-SL\n"
												"AGC SLOW\n"
												"AGC OFF");
	lv_obj_remove_style_all(radioAGCDropDown);
	lv_obj_add_style(radioAGCDropDown, &mainStyle, 0);
	lv_dropdown_set_symbol(radioAGCDropDown, NULL);
	lv_obj_set_size(radioAGCDropDown, 100, 30);
	lv_obj_set_pos(radioAGCDropDown, 120, 105);
	//Style Drop Down list items
	list = lv_dropdown_get_list(radioAGCDropDown);
	lv_obj_remove_style_all(list);
	lv_obj_add_style(list, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(list, &mainStyleSelected, LV_PART_SELECTED);
	lv_obj_set_style_text_line_space(list, 6, LV_PART_MAIN);
	//Add event
	lv_obj_add_event_cb(radioAGCDropDown, GUIBasicRadioConfigDropDownEvent, LV_EVENT_VALUE_CHANGED, NULL);

	//Radio AFC Enable Button
	radioAFCEnableButton = lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(radioAFCEnableButton);
	lv_obj_add_style(radioAFCEnableButton, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(radioAFCEnableButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_add_style(radioAFCEnableButton, &mainStyleDisabled, LV_PART_MAIN);
	lv_obj_add_style(radioAFCEnableButton, &mainStyleEnabled, LV_PART_MAIN | LV_STATE_CHECKED);
	lv_obj_add_flag(radioAFCEnableButton, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_add_state(radioAFCEnableButton, LV_STATE_CHECKED);
	lv_obj_set_size(radioAFCEnableButton, 60, 30);
	lv_obj_set_pos(radioAFCEnableButton, 220, 105);
	//Button Label
	lv_obj_t* label = lv_label_create(radioAFCEnableButton);
	lv_label_set_text(label, "AFC");
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_center(label);
	//Add event
	lv_obj_add_event_cb(radioAFCEnableButton, GUIBasicRadioConfigButtonEvent, LV_EVENT_VALUE_CHANGED, NULL);

	//Radio TNC Enable Button
	radioTNCEnableButton = lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(radioTNCEnableButton);
	lv_obj_add_style(radioTNCEnableButton, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(radioTNCEnableButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_add_style(radioTNCEnableButton, &mainStyleDisabled, LV_PART_MAIN);
	lv_obj_add_style(radioTNCEnableButton, &mainStyleEnabled, LV_PART_MAIN | LV_STATE_CHECKED);
	lv_obj_add_flag(radioTNCEnableButton, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_add_state(radioTNCEnableButton, LV_STATE_CHECKED);
	lv_obj_set_size(radioTNCEnableButton, 60, 30);
	lv_obj_set_pos(radioTNCEnableButton, 280, 105);
	//Button Label
	label = lv_label_create(radioTNCEnableButton);
	lv_label_set_text(label, "TNC");
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_center(label);
	//Add event
	lv_obj_add_event_cb(radioTNCEnableButton, GUIBasicRadioConfigButtonEvent, LV_EVENT_VALUE_CHANGED, NULL);
}
