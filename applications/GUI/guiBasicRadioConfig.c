#include "guiBasicRadioConfig.h"

//Radio Basic Configuration GUI Objects
lv_obj_t* radioModeDropDown;
lv_obj_t* radioModulationDropDown;
lv_obj_t* radioAGCDropDown;
lv_obj_t* radioAFCDropDown;
lv_obj_t* radioAFCEnableButton;
lv_obj_t* radioPacketConfigurationButton;
lv_obj_t* radioPacketConfigurationMenu;
lv_obj_t* radioPacketEncodingDropDown;
lv_obj_t* radioPacketFramingDropDown;
lv_obj_t* radioPacketCRCDropDown;
lv_obj_t* radioRXDatarateTextArea;
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
		else if(btn == radioPacketConfigurationButton) {
			if(lv_obj_has_state(btn, LV_STATE_CHECKED) == true) {
				//Show Packet Menu
				lv_obj_clear_flag(radioPacketConfigurationMenu, LV_OBJ_FLAG_HIDDEN);
				lv_obj_move_foreground(radioPacketConfigurationMenu);
			}
			else {
				//Hide Packet Menu
				lv_obj_add_flag(radioPacketConfigurationMenu, LV_OBJ_FLAG_HIDDEN);
			}
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
		else if(obj == radioPacketEncodingDropDown) {
			uint16_t selected = lv_dropdown_get_selected(obj);
			//Inform Radio Interface of value change
			InterThreadMessageStruct msg = {.id = InterThread_Encoding, .data = (uint32_t*)selected, .length = 0 };
			rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
		}
		else if(obj == radioPacketFramingDropDown) {
			uint16_t selected = lv_dropdown_get_selected(obj);
			//Inform Radio Interface of value change
			InterThreadMessageStruct msg = {.id = InterThread_Framing, .data = (uint32_t*)selected, .length = 0 };
			rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
		}
		else if(obj == radioPacketCRCDropDown) {
			uint16_t selected = lv_dropdown_get_selected(obj);
			//Inform Radio Interface of value change
			InterThreadMessageStruct msg = {.id = InterThread_CRC, .data = (uint32_t*)selected, .length = 0 };
			rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
		}
	}
}

/**
  * @brief	This function handles Text Area events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
char rxDatarateTextAreaPrevStr[20];
static void GUIBasicRadioConfigTextArea(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    const char * txt = lv_textarea_get_text(ta);
    if(code == LV_EVENT_FOCUSED) {
    	//Get current string
    	strcpy(rxDatarateTextAreaPrevStr, txt);
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(kb);
    }

    if(code == LV_EVENT_DEFOCUSED) {
    	//Cancel input value, recover old input
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        if(rxDatarateTextAreaPrevStr[0] != 0x00) {
        	strcpy(txt, rxDatarateTextAreaPrevStr);
        }
    }

    if(code == LV_EVENT_READY) {
    	//Keyboard OK symbol pressed, save and apply input value
    	lv_obj_clear_state(ta, LV_STATE_FOCUSED);
    	lv_keyboard_set_textarea(kb, NULL);
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
		rxDatarateTextAreaPrevStr[0] = 0x00;

		if(ta == radioRXDatarateTextArea) {
			//Center Frequency Text Area Change apply
			uint32_t value;
			if(sscanf(txt, "%d", &value) == 1) {
				//Inform Radio Interface of value change
				InterThreadMessageStruct msg = {.id = InterThread_DatarateRX, .data = (uint32_t*)value, .length = 0 };
				rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));
			}
		}
    }

    if(code == LV_EVENT_VALUE_CHANGED) {
    	//Value changed
    }
}

/**
  * @brief	This function handles Packet Configuration Defocus events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
static void GUIBasicRadioPktMenuDeFocusEvent(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t * obj = lv_event_get_target(e);
	if(code == LV_EVENT_DEFOCUSED) {
		if(obj == radioPacketConfigurationMenu) {
			//Hide Packet Menu
			lv_obj_add_flag(radioPacketConfigurationMenu, LV_OBJ_FLAG_HIDDEN);
			lv_obj_clear_state(radioPacketConfigurationButton, LV_STATE_CHECKED);
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
	lv_dropdown_set_options(radioAGCDropDown,	"AGC FST\n"
												"AGC M-F\n"
												"AGC MED\n"
												"AGC M-s\n"
												"AGC SLW\n"
												"AGC OFF");
	lv_obj_remove_style_all(radioAGCDropDown);
	lv_obj_add_style(radioAGCDropDown, &mainStyle, 0);
	lv_dropdown_set_symbol(radioAGCDropDown, NULL);
	lv_obj_set_size(radioAGCDropDown, 80, 30);
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
	lv_obj_set_pos(radioAFCEnableButton, 200, 105);
	//Button Label
	lv_obj_t* label = lv_label_create(radioAFCEnableButton);
	lv_label_set_text(label, "AFC");
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_center(label);
	//Add event
	lv_obj_add_event_cb(radioAFCEnableButton, GUIBasicRadioConfigButtonEvent, LV_EVENT_VALUE_CHANGED, NULL);

	//Radio RX Datarate Text Area
	//Text Area
	radioRXDatarateTextArea = lv_textarea_create(lv_scr_act());
	lv_obj_remove_style_all(radioRXDatarateTextArea);
	lv_obj_add_style(radioRXDatarateTextArea, &mainStyle, LV_PART_MAIN);
	lv_textarea_set_one_line(radioRXDatarateTextArea, true);
	lv_textarea_set_max_length(radioRXDatarateTextArea, 6);
	lv_obj_set_style_text_align(radioRXDatarateTextArea, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
//	lv_obj_set_style_border_width(radioRXDatarateTextArea, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(radioRXDatarateTextArea, 2, LV_PART_CURSOR | LV_STATE_FOCUSED);
	lv_obj_set_style_border_side(radioRXDatarateTextArea, LV_BORDER_SIDE_LEFT, LV_PART_CURSOR);
	lv_obj_set_style_border_color(radioRXDatarateTextArea, STYLE_COLOR_GREY, LV_PART_CURSOR);
	lv_obj_set_size(radioRXDatarateTextArea, 90, 30);
	lv_obj_set_pos(radioRXDatarateTextArea, 260, 105);
	lv_textarea_set_text(radioRXDatarateTextArea, "250000");
	lv_obj_add_event_cb(radioRXDatarateTextArea, GUIBasicRadioConfigTextArea, LV_EVENT_FOCUSED, numPad);
	lv_obj_add_event_cb(radioRXDatarateTextArea, GUIBasicRadioConfigTextArea, LV_EVENT_DEFOCUSED, numPad);
	lv_obj_add_event_cb(radioRXDatarateTextArea, GUIBasicRadioConfigTextArea, LV_EVENT_READY, numPad);
	//Label
	label = lv_label_create(lv_scr_act());
	lv_label_set_text(label, "DR");
	lv_obj_remove_style_all(label);
	lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_right(label, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_left(label, 1, LV_PART_MAIN);
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_set_style_bg_color(label, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_size(label, 28, 30);
	lv_obj_set_pos(label, 260, 107);

	//Radio Packet Configuration Button
	radioPacketConfigurationButton = lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(radioPacketConfigurationButton);
	lv_obj_add_style(radioPacketConfigurationButton, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(radioPacketConfigurationButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_add_style(radioPacketConfigurationButton, &mainStyleSelected, LV_PART_MAIN | LV_STATE_CHECKED);
	lv_obj_add_flag(radioPacketConfigurationButton, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_set_size(radioPacketConfigurationButton, 80, 30);
	lv_obj_set_pos(radioPacketConfigurationButton, 350, 105);
	//Button Label
	label = lv_label_create(radioPacketConfigurationButton);
	lv_label_set_text(label, "PACKET");
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_center(label);
	//Add event
	lv_obj_add_event_cb(radioPacketConfigurationButton, GUIBasicRadioConfigButtonEvent, LV_EVENT_VALUE_CHANGED, NULL);

	//Radio Packet Configuration Menu
	radioPacketConfigurationMenu = lv_obj_create(lv_scr_act());
	lv_obj_remove_style_all(radioPacketConfigurationMenu);
	lv_obj_add_style(radioPacketConfigurationMenu, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(radioPacketConfigurationMenu, 0, LV_PART_MAIN);
	lv_obj_set_size(radioPacketConfigurationMenu, 175, 102);
	lv_obj_set_pos(radioPacketConfigurationMenu, 305, 135);
	lv_obj_add_flag(radioPacketConfigurationMenu, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_event_cb(radioPacketConfigurationMenu, GUIBasicRadioPktMenuDeFocusEvent, LV_EVENT_DEFOCUSED, NULL);
	//Add Encoding Setting
	//Label
	label = lv_label_create(radioPacketConfigurationMenu);
	lv_label_set_text(label, "ENCODER: ");
	lv_obj_remove_style_all(label);
	lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(label, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_size(label, 90, 30);
	lv_obj_set_pos(label, 2, 2);
	//Drop Down
	radioPacketEncodingDropDown = lv_dropdown_create(radioPacketConfigurationMenu);
	lv_dropdown_set_options(radioPacketEncodingDropDown,	"NRZ\n"
															"NRZ-S\n"
															"NRZI\n"
															"NRZI-S\n"
															"FM1\n"
															"FM0\n"
															"MAN");
	lv_obj_remove_style_all(radioPacketEncodingDropDown);
	lv_obj_add_style(radioPacketEncodingDropDown, &mainStyle, 0);
	lv_dropdown_set_symbol(radioPacketEncodingDropDown, NULL);
	lv_obj_set_size(radioPacketEncodingDropDown, 80, 30);
	lv_obj_set_pos(radioPacketEncodingDropDown, 90, 2);
	//Style Drop Down list items
	list = lv_dropdown_get_list(radioPacketEncodingDropDown);
	lv_obj_remove_style_all(list);
	lv_obj_add_style(list, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(list, &mainStyleSelected, LV_PART_SELECTED);
	lv_obj_set_style_text_line_space(list, 6, LV_PART_MAIN);
	//Add event
	lv_obj_add_event_cb(radioPacketEncodingDropDown, GUIBasicRadioConfigDropDownEvent, LV_EVENT_VALUE_CHANGED, NULL);
	//Add Framing Setting
	//Label
	label = lv_label_create(radioPacketConfigurationMenu);
	lv_label_set_text(label, "FRAMING: ");
	lv_obj_remove_style_all(label);
	lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(label, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_size(label, 90, 30);
	lv_obj_set_pos(label, 2, 34);
	//Drop Down
	radioPacketFramingDropDown = lv_dropdown_create(radioPacketConfigurationMenu);
	lv_dropdown_set_options(radioPacketFramingDropDown,	"RAW\n"
														"HDLC\n"
														"W-MBUS");
	lv_obj_remove_style_all(radioPacketFramingDropDown);
	lv_obj_add_style(radioPacketFramingDropDown, &mainStyle, 0);
	lv_dropdown_set_symbol(radioPacketFramingDropDown, NULL);
	lv_obj_set_size(radioPacketFramingDropDown, 80, 30);
	lv_obj_set_pos(radioPacketFramingDropDown, 90, 34);
	//Style Drop Down list items
	list = lv_dropdown_get_list(radioPacketFramingDropDown);
	lv_obj_remove_style_all(list);
	lv_obj_add_style(list, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(list, &mainStyleSelected, LV_PART_SELECTED);
	lv_obj_set_style_text_line_space(list, 6, LV_PART_MAIN);
	//Add event
	lv_obj_add_event_cb(radioPacketFramingDropDown, GUIBasicRadioConfigDropDownEvent, LV_EVENT_VALUE_CHANGED, NULL);
	//Add CRC Setting
	//Label
	label = lv_label_create(radioPacketConfigurationMenu);
	lv_label_set_text(label, "CRC: ");
	lv_obj_remove_style_all(label);
	lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(label, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_size(label, 90, 30);
	lv_obj_set_pos(label, 2, 66);
	//Drop Down
	radioPacketCRCDropDown = lv_dropdown_create(radioPacketConfigurationMenu);
	lv_dropdown_set_options(radioPacketCRCDropDown,	"OFF\n"
													"CCITT\n"
													"CRC16\n"
													"DNP\n"
													"CRC32");
	lv_obj_remove_style_all(radioPacketCRCDropDown);
	lv_obj_add_style(radioPacketCRCDropDown, &mainStyle, 0);
	lv_dropdown_set_symbol(radioPacketCRCDropDown, NULL);
	lv_obj_set_size(radioPacketCRCDropDown, 80, 30);
	lv_obj_set_pos(radioPacketCRCDropDown, 90, 66);
	//Style Drop Down list items
	list = lv_dropdown_get_list(radioPacketCRCDropDown);
	lv_obj_remove_style_all(list);
	lv_obj_add_style(list, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(list, &mainStyleSelected, LV_PART_SELECTED);
	lv_obj_set_style_text_line_space(list, 6, LV_PART_MAIN);
	//Add event
	lv_obj_add_event_cb(radioPacketCRCDropDown, GUIBasicRadioConfigDropDownEvent, LV_EVENT_VALUE_CHANGED, NULL);

	//Radio TNC Enable Button
	radioTNCEnableButton = lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(radioTNCEnableButton);
	lv_obj_add_style(radioTNCEnableButton, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(radioTNCEnableButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_add_style(radioTNCEnableButton, &mainStyleDisabled, LV_PART_MAIN);
	lv_obj_add_style(radioTNCEnableButton, &mainStyleEnabled, LV_PART_MAIN | LV_STATE_CHECKED);
	lv_obj_add_flag(radioTNCEnableButton, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_add_state(radioTNCEnableButton, LV_STATE_CHECKED);
	lv_obj_set_size(radioTNCEnableButton, 50, 30);
	lv_obj_set_pos(radioTNCEnableButton, 430, 105);
	//Button Label
	label = lv_label_create(radioTNCEnableButton);
	lv_label_set_text(label, "TNC");
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_center(label);
	//Add event
	lv_obj_add_event_cb(radioTNCEnableButton, GUIBasicRadioConfigButtonEvent, LV_EVENT_VALUE_CHANGED, NULL);
}
