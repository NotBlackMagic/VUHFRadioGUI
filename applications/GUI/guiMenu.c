#include "guiMenu.h"

#define GUI_HARDWARE_VERSION_MAJOR				1
#define GUI_HARDWARE_VERSION_MINOR				0
#define GUI_FIRMWARE_VERSION_MAJOR				1
#define GUI_FIRMWARE_VERSION_MINOR				14
#define GUI_FIRMWARE_VERSION_HASH				0x0f839b8e

//Main menu object
lv_obj_t* menu;
//Menu sub-pages objects
lv_obj_t* synthSubPage;
lv_obj_t* basebandSubPage;
lv_obj_t* rxSettingSubPage;
lv_obj_t* modulationSubPage;
lv_obj_t* framingSubPage;
lv_obj_t* packetSubPage;
lv_obj_t* trackingSubPage;
lv_obj_t* experimentalSubPage;
lv_obj_t* aboutSubPage;


lv_obj_t* agcSpeedDropDown;
lv_obj_t* agcTargetTextArea;

lv_obj_t* aboutMenuLabel;

lv_obj_t* GUIMenuCreateMenuEntry(lv_obj_t* parent, char* text);
lv_obj_t* GUIMenuCreateDropDown(lv_obj_t* parent, char* text, char* options);
lv_obj_t* GUIMenuCreateTextArea(lv_obj_t* parent, char* text, uint8_t maxChars, char* defaultText);

lv_obj_t* GUISynthMenuSubPage(lv_obj_t* parent);
lv_obj_t* GUIBasebandMenuSubPage(lv_obj_t* parent);
lv_obj_t* GUIRXSettingsMenuSubPage(lv_obj_t* parent);
lv_obj_t* GUIModulationMenuSubPage(lv_obj_t* parent);
lv_obj_t* GUIFramingMenuSubPage(lv_obj_t* parent);
lv_obj_t* GUIPacketMenuSubPage(lv_obj_t* parent);
lv_obj_t* GUITrackingMenuSubPage(lv_obj_t* parent);
lv_obj_t* GUIExperimentalMenuSubPage(lv_obj_t* parent);
lv_obj_t* GUIAboutMenuSubPage(lv_obj_t* parent);

void GUIAboutMenuSubPageUpdate();

static void GUIMenuPageChanged(lv_event_t* e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* obj = lv_event_get_target(e);
	lv_obj_t* subPage = lv_menu_get_cur_main_page(obj);
	if(code == LV_EVENT_VALUE_CHANGED) {
		if(subPage == aboutSubPage) {
			GUIAboutMenuSubPageUpdate();
		}
	}
}

/**
  * @brief	This function handles Drop Down events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
static void GUIMenuDropDownEvent(lv_event_t* e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* obj = lv_event_get_target(e);
	lv_obj_t* parent = lv_obj_get_parent(obj);
	if(code == LV_EVENT_VALUE_CHANGED) {
		if(parent == agcSpeedDropDown) {
			uint16_t selected = lv_dropdown_get_selected(obj);
		}
	}
}

/**
  * @brief	This function handles Text Area events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
char textAreaPrevStr[20];
static void GUIMenuTextArea(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* ta = lv_event_get_target(e);
    lv_obj_t* parent = lv_obj_get_parent(e);
    lv_obj_t* kb = lv_event_get_user_data(e);
    const char* txt = lv_textarea_get_text(ta);
    if(code == LV_EVENT_FOCUSED) {
    	//Get current string
    	strcpy(textAreaPrevStr, txt);
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(kb);
    }

    if(code == LV_EVENT_DEFOCUSED) {
    	//Cancel input value, recover old input
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        if(textAreaPrevStr[0] != 0x00) {
        	strcpy(txt, textAreaPrevStr);
        }
    }

    if(code == LV_EVENT_READY) {
    	//Keyboard OK symbol pressed, save and apply input value
    	lv_obj_clear_state(ta, LV_STATE_FOCUSED);
    	lv_keyboard_set_textarea(kb, NULL);
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
		textAreaPrevStr[0] = 0x00;

		if(parent == agcTargetTextArea) {
			//Text Area Change apply
			uint32_t value;
			if(sscanf(txt, "%d", &value) == 1) {
				//Inform Radio Interface of value change
			}
		}
    }

    if(code == LV_EVENT_VALUE_CHANGED) {
    	//Value changed
    }
}

void GUIMenuInit() {
	//Set Menu
	//Create Menu Object
	menu = lv_menu_create(lv_scr_act());
//	lv_obj_remove_style_all(menu);
	lv_obj_add_style(menu, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(menu, 1, LV_PART_MAIN);
	lv_obj_set_style_bg_color(menu, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_outline_color(menu, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_outline_width(menu, 2, LV_PART_MAIN);
	lv_obj_set_style_pad_all(menu, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_gap(menu, 0, LV_PART_MAIN);
	lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_DISABLED);
	lv_obj_set_size(menu, 478, 243);
	lv_obj_set_pos(menu, 1, 1);
	lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);

	//Create Side Bar page
	lv_obj_t* mainPage = lv_menu_page_create(menu, NULL);
	lv_obj_add_style(mainPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_side(mainPage, LV_BORDER_SIDE_RIGHT, LV_PART_MAIN);
	lv_obj_set_style_border_color(mainPage, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_set_style_bg_color(mainPage, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_pad_all(mainPage, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_gap(mainPage, 0, LV_PART_MAIN);
	lv_obj_set_style_height(mainPage, lv_pct(100), LV_PART_MAIN);
	lv_menu_set_sidebar_page(menu, mainPage);
	//Remove Side Bar Header
	lv_obj_t* header = lv_menu_get_sidebar_header(menu);
	lv_obj_set_height(header, 0);

	lv_obj_t* cont;

	//Create Sub pages for each menu section
	synthSubPage = GUISynthMenuSubPage(menu);
	basebandSubPage = GUIBasebandMenuSubPage(menu);
	rxSettingSubPage = GUIRXSettingsMenuSubPage(menu);
	modulationSubPage = GUIModulationMenuSubPage(menu);
	framingSubPage = GUIFramingMenuSubPage(menu);
	packetSubPage = GUIPacketMenuSubPage(menu);
	trackingSubPage = GUITrackingMenuSubPage(menu);
	experimentalSubPage = GUIExperimentalMenuSubPage(menu);
	aboutSubPage = GUIAboutMenuSubPage(menu);

	lv_obj_t * digitalSubPage = lv_menu_page_create(menu, NULL);
	lv_obj_add_style(digitalSubPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(digitalSubPage, 0, LV_PART_MAIN);
	lv_obj_set_style_border_side(digitalSubPage, LV_BORDER_SIDE_LEFT, LV_PART_MAIN);
	cont = GUIMenuCreateMenuEntry(digitalSubPage, "Datarate");
	cont = GUIMenuCreateMenuEntry(digitalSubPage, "Encoding");
	cont = GUIMenuCreateMenuEntry(digitalSubPage, "Framing");
	cont = GUIMenuCreateMenuEntry(digitalSubPage, "CRC");

	//Create Menu Sections in Main page
	cont = GUIMenuCreateMenuEntry(mainPage, "Synthesizer");
	lv_menu_set_load_page_event(menu, cont, synthSubPage);
	cont = GUIMenuCreateMenuEntry(mainPage, "Baseband");
	lv_menu_set_load_page_event(menu, cont, basebandSubPage);
	cont = GUIMenuCreateMenuEntry(mainPage, "RX Settings");
	lv_menu_set_load_page_event(menu, cont, rxSettingSubPage);
	cont = GUIMenuCreateMenuEntry(mainPage, "Modulation");
	lv_menu_set_load_page_event(menu, cont, modulationSubPage);
	cont = GUIMenuCreateMenuEntry(mainPage, "Framing");
	lv_menu_set_load_page_event(menu, cont, framingSubPage);
	cont = GUIMenuCreateMenuEntry(mainPage, "Packet");
	lv_menu_set_load_page_event(menu, cont, packetSubPage);
	cont = GUIMenuCreateMenuEntry(mainPage, "Tracking");
	lv_menu_set_load_page_event(menu, cont, trackingSubPage);
	cont = GUIMenuCreateMenuEntry(mainPage, "Experimental");
	lv_menu_set_load_page_event(menu, cont, experimentalSubPage);
	cont = GUIMenuCreateMenuEntry(mainPage, "About");
	lv_menu_set_load_page_event(menu, cont, aboutSubPage);

	//Open by default first page after root
//    lv_event_send(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), 0), LV_EVENT_CLICKED, NULL);
	lv_event_send(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), LV_EVENT_CLICKED, NULL);

	//Add event, only at end when sub-pages are already created!
	lv_obj_add_event_cb(menu, GUIMenuPageChanged, LV_EVENT_VALUE_CHANGED, NULL);
}

lv_obj_t* GUISynthMenuSubPage(lv_obj_t* parent) {
	lv_obj_t* subPage = lv_menu_page_create(parent, NULL);
	lv_obj_add_style(subPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(subPage, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(subPage, 0, LV_PART_MAIN);

	//Create Header
	lv_obj_t* header = lv_label_create(subPage);
	lv_obj_remove_style_all(header);
	lv_obj_add_style(header, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_color(header, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
	lv_obj_set_style_bg_color(header, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_width(header, lv_pct(100), LV_PART_MAIN);
    lv_obj_set_style_text_align(header, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(header, "Synthesizer/PLL");

	lv_obj_t* synthCF = GUIMenuCreateTextArea(subPage, "Center Frequency:", 9, "433000000");
	lv_obj_t* synthPLLFilter = GUIMenuCreateDropDown(subPage, "Filter:", 	"EXTERNAL\n"
																			"100 kHz\n"
																			"200 kHz\n"
																			"500 kHz");
	lv_obj_t* synthPLLRangeClock = GUIMenuCreateDropDown(subPage, "Range Clock:", 	"1/256\n"
																					"1/512\n"
																					"1/1024\n"
																					"1/2048\n"
																					"1/4096\n"
																					"1/8192\n"
																					"1/16384\n"
																					"1/32768");
	lv_obj_t* synthPLLChargePump = GUIMenuCreateTextArea(subPage, "Charge Pump:", 3, "8");
	lv_obj_t* synthPLLChargePumpBoost = GUIMenuCreateTextArea(subPage, "Charge Pump Boost:", 3, "200");

	return subPage;
}

lv_obj_t* GUIBasebandMenuSubPage(lv_obj_t* parent) {
	lv_obj_t * subPage = lv_menu_page_create(parent, NULL);
	lv_obj_add_style(subPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(subPage, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(subPage, 0, LV_PART_MAIN);

	//Create Header
	lv_obj_t* header = lv_label_create(subPage);
	lv_obj_remove_style_all(header);
	lv_obj_add_style(header, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_color(header, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
	lv_obj_set_style_bg_color(header, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_width(header, lv_pct(100), LV_PART_MAIN);
	lv_obj_set_style_text_align(header, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(header, "Baseband");

	lv_obj_t* basebandIF = GUIMenuCreateTextArea(subPage, "IF Frequency:", 6, "7500");
	lv_obj_t* basebandBW = GUIMenuCreateTextArea(subPage, "Bandwidth:", 6, "15000");
	lv_obj_t* basebandRelative = GUIMenuCreateDropDown(subPage, "BW Fraction:",	"0.150\n"
																				"0.178\n"
																				"0.211\n"
																				"0.250");
	lv_obj_t* basebandLeak = GUIMenuCreateTextArea(subPage, "Leakiness:", 2, "0");

	return subPage;
}

lv_obj_t* GUIRXSettingsMenuSubPage(lv_obj_t* parent) {
	lv_obj_t * subPage = lv_menu_page_create(parent, NULL);
	lv_obj_add_style(subPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(subPage, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(subPage, 0, LV_PART_MAIN);

	//Create Header
	lv_obj_t* header = lv_label_create(subPage);
	lv_obj_remove_style_all(header);
	lv_obj_add_style(header, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_color(header, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
	lv_obj_set_style_bg_color(header, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_width(header, lv_pct(100), LV_PART_MAIN);
	lv_obj_set_style_text_align(header, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(header, "Receiver Settings");

//	lv_obj_t* subPageTabs = lv_tabview_create(subPage, LV_DIR_TOP, 30);
//	lv_obj_set_style_bg_color(subPageTabs, STYLE_COLOR_BLACK, LV_PART_MAIN);
//	lv_obj_set_style_pad_all(subPageTabs, 0, LV_PART_MAIN);
////	lv_obj_add_event_cb(lv_tabview_get_content(subPageTabs), scroll_begin_event, LV_EVENT_SCROLL_BEGIN, NULL);
//
//	lv_obj_t * tab_btns = lv_tabview_get_tab_btns(subPageTabs);
//	lv_obj_add_style(tab_btns, &mainStyle, LV_PART_MAIN);
//	lv_obj_set_style_pad_all(tab_btns, 0, LV_PART_MAIN);
//	lv_obj_set_style_pad_gap(tab_btns, 0, LV_PART_MAIN);
//	lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
//	lv_obj_add_style(tab_btns, &mainStyleSelected, LV_PART_ITEMS | LV_STATE_CHECKED);
//	lv_obj_set_style_border_width(tab_btns, 0, LV_PART_ITEMS | LV_STATE_CHECKED);
//
//	lv_obj_t* agcTabPage = lv_tabview_add_tab(subPageTabs, "AGC");
//	lv_obj_set_style_pad_all(agcTabPage, 0, LV_PART_MAIN);
//	lv_obj_set_style_pad_top(agcTabPage, 2, LV_PART_MAIN);
//	lv_obj_set_layout(agcTabPage, LV_LAYOUT_FLEX);
//	lv_obj_set_flex_flow(agcTabPage, LV_FLEX_FLOW_COLUMN);
	agcSpeedDropDown = GUIMenuCreateDropDown(subPage, "AGC Speed:", 	"AGC FST\n"
																		"AGC M-F\n"
																		"AGC MED\n"
																		"AGC M-s\n"
																		"AGC SLW\n"
																		"AGC OFF");
	agcTargetTextArea = GUIMenuCreateTextArea(subPage, "AGC Target:", 6, "256");

//	lv_obj_t* afcTabPage = lv_tabview_add_tab(subPage, "AFC");
	lv_obj_t* afcRangeDropDown = GUIMenuCreateTextArea(subPage, "AFC Range:", 6, "3750");

	lv_obj_t* afskBW = GUIMenuCreateTextArea(subPage, "AFSK BW:", 6, "0");

	return subPage;
}

lv_obj_t* GUIModulationMenuSubPage(lv_obj_t* parent) {
	//Create Sub-Page
	lv_obj_t * subPage = lv_menu_page_create(parent, NULL);
	lv_obj_add_style(subPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(subPage, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(subPage, 0, LV_PART_MAIN);

	//Create Header
	lv_obj_t* header = lv_label_create(subPage);
	lv_obj_remove_style_all(header);
	lv_obj_add_style(header, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_color(header, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
	lv_obj_set_style_bg_color(header, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_width(header, lv_pct(100), LV_PART_MAIN);
	lv_obj_set_style_text_align(header, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(header, "Modulation");

	lv_obj_t* modulationMode = GUIMenuCreateDropDown(subPage, "Mode:", 	"ASK\n"
																		"ASK Co.\n"
																		"PSK\n"
																		"OQPSK\n"
																		"MSK\n"
																		"FSK\n"
																		"4-FSK\n"
																		"AFSK\n"
																		"FM");
	lv_obj_t* modulationDatarate = GUIMenuCreateTextArea(subPage, "Datarate:", 6, "1200");
	lv_obj_t* modulationFrequencyShaping = GUIMenuCreateDropDown(subPage, "Frequency Shaping:", "OFF\n"
																								"GS 0.3\n"
																								"GS 0.5");
	lv_obj_t* modulationAmplitudeShaping = GUIMenuCreateDropDown(subPage, "Amplitude Shaping:", "OFF\n"
																								"Raised Cosine");
	lv_obj_t* modulationFSKDeviation = GUIMenuCreateTextArea(subPage, "FSK Deviation:", 6, "1200");
	lv_obj_t* modulationFMDeviation = GUIMenuCreateTextArea(subPage, "FM Deviation:", 6, "0");
	lv_obj_t* modulationAFSKDeviation = GUIMenuCreateTextArea(subPage, "AFSK Deviation:", 6, "0");
	lv_obj_t* modulationAFSKSpace = GUIMenuCreateTextArea(subPage, "AFSK Space:", 6, "0");
	lv_obj_t* modulationAFSKMark = GUIMenuCreateTextArea(subPage, "AFSK Mark:", 6, "0");

	return subPage;
}

lv_obj_t* GUIFramingMenuSubPage(lv_obj_t* parent) {
	lv_obj_t * subPage = lv_menu_page_create(parent, NULL);
	lv_obj_add_style(subPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(subPage, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(subPage, 0, LV_PART_MAIN);

	//Create Header
	lv_obj_t* header = lv_label_create(subPage);
	lv_obj_remove_style_all(header);
	lv_obj_add_style(header, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_color(header, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
	lv_obj_set_style_bg_color(header, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_width(header, lv_pct(100), LV_PART_MAIN);
	lv_obj_set_style_text_align(header, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(header, "Framing");

	return subPage;
}

lv_obj_t* GUIPacketMenuSubPage(lv_obj_t* parent) {
	lv_obj_t * subPage = lv_menu_page_create(parent, NULL);
	lv_obj_add_style(subPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(subPage, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(subPage, 0, LV_PART_MAIN);

	//Create Header
	lv_obj_t* header = lv_label_create(subPage);
	lv_obj_remove_style_all(header);
	lv_obj_add_style(header, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_color(header, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
	lv_obj_set_style_bg_color(header, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_width(header, lv_pct(100), LV_PART_MAIN);
	lv_obj_set_style_text_align(header, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(header, "Packet");

	return subPage;
}

lv_obj_t* GUITrackingMenuSubPage(lv_obj_t* parent) {
	lv_obj_t * subPage = lv_menu_page_create(parent, NULL);
	lv_obj_add_style(subPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(subPage, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(subPage, 0, LV_PART_MAIN);

	//Create Header
	lv_obj_t* header = lv_label_create(subPage);
	lv_obj_remove_style_all(header);
	lv_obj_add_style(header, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_color(header, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
	lv_obj_set_style_bg_color(header, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_width(header, lv_pct(100), LV_PART_MAIN);
	lv_obj_set_style_text_align(header, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(header, "Tracking");

	return subPage;
}

lv_obj_t* GUIExperimentalMenuSubPage(lv_obj_t* parent) {
	lv_obj_t * subPage = lv_menu_page_create(parent, NULL);
	lv_obj_add_style(subPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(subPage, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(subPage, 0, LV_PART_MAIN);

	//Create Header
	lv_obj_t* header = lv_label_create(subPage);
	lv_obj_remove_style_all(header);
	lv_obj_add_style(header, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_color(header, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
	lv_obj_set_style_bg_color(header, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_width(header, lv_pct(100), LV_PART_MAIN);
	lv_obj_set_style_text_align(header, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(header, "Experimental Features");

	return subPage;
}

lv_obj_t* GUIAboutMenuSubPage(lv_obj_t* parent) {
	lv_obj_t* subPage = lv_menu_page_create(parent, NULL);
	lv_obj_add_style(subPage, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(subPage, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(subPage, 0, LV_PART_MAIN);

	//Create Header
	lv_obj_t* header = lv_label_create(subPage);
	lv_obj_remove_style_all(header);
	lv_obj_add_style(header, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_color(header, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
	lv_obj_set_style_bg_color(header, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_width(header, lv_pct(100), LV_PART_MAIN);
    lv_obj_set_style_text_align(header, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(header, "About");

	//Create the Information Label
	aboutMenuLabel = lv_label_create(subPage);
	lv_obj_remove_style_all(aboutMenuLabel);
	lv_obj_add_style(aboutMenuLabel, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(aboutMenuLabel, 0, LV_PART_MAIN);
	lv_obj_align(aboutMenuLabel, LV_ALIGN_TOP_LEFT, 0, 0);
	lv_obj_set_style_text_align(aboutMenuLabel, LV_TEXT_ALIGN_LEFT, 0);
	lv_label_set_text(aboutMenuLabel, "NONE");

	return subPage;
}

/**
  * @brief	This function is used to update information when this menu window is opened
  * @param	None
  * @return	None
  */
void GUIAboutMenuSubPageUpdate(lv_event_t * e) {
	//Update text
	uint16_t index = 0;
	char str[200];
	index += sprintf(&str[index], "GUI Module\n");
	index += sprintf(&str[index], "HW: %d.%02d\n", GUI_HARDWARE_VERSION_MAJOR, GUI_HARDWARE_VERSION_MINOR);
	index += sprintf(&str[index], "SW: %d.%02d (0x%08X)\n\n", GUI_FIRMWARE_VERSION_MAJOR, GUI_FIRMWARE_VERSION_MINOR, GUI_FIRMWARE_VERSION_HASH);
	index += sprintf(&str[index], "VUHFRadio Module\n");
	index += sprintf(&str[index], "HW: %d.%02d\n", hardwareVersionMajor, hardwareVersionMinor);
	index += sprintf(&str[index], "SW: %d.%02d (0x%08X)\n\n", firmwareVersionMajor, firmwareVersionMinor, firmwareVersionHash);
	index += sprintf(&str[index], "CAT INTERFACE\n");
	index += sprintf(&str[index], "SW: %d.%02d (0x%08X)\n", catInterfaceVersionMajor, catInterfaceVersionMinor, catInterfaceVersionHash);

	lv_label_set_text(aboutMenuLabel, str);
}

lv_obj_t* GUIMenuCreateMenuEntry(lv_obj_t* parent, char* text) {
	lv_obj_t* cont = lv_menu_cont_create(parent);
	lv_obj_remove_style_all(cont);
	lv_obj_add_style(cont, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
	lv_obj_set_style_bg_color(cont, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_add_style(cont, &mainStylePressed, LV_PART_MAIN | LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(cont, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN | LV_STATE_CHECKED);
	lv_obj_t* label = lv_label_create(cont);
	lv_label_set_text(label, text);
	return cont;
}

lv_obj_t* GUIMenuCreateDropDown(lv_obj_t* parent, char* text, char* options) {
	lv_obj_t* cont = lv_menu_cont_create(parent);
	lv_obj_remove_style_all(cont);
	lv_obj_add_style(cont, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(cont, &mainStylePressed, LV_PART_MAIN | LV_STATE_CHECKED);
	lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
//	lv_obj_set_style_bg_color(cont, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_set_style_pad_all(cont, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_left(cont, 2, LV_PART_MAIN);
	lv_obj_set_height(cont, 30);
	lv_obj_t* label = lv_label_create(cont);
	lv_label_set_text(label, text);
	lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);

	//Drop Down
	lv_obj_t* dropDown = lv_dropdown_create(cont);
	lv_dropdown_set_options(dropDown, options);
	lv_obj_remove_style_all(dropDown);
	lv_obj_add_style(dropDown, &mainStyle, LV_PART_MAIN);
	lv_dropdown_set_symbol(dropDown, NULL);
	lv_obj_set_size(dropDown, 100, 30);
	lv_obj_align(dropDown, LV_ALIGN_RIGHT_MID, 0, 0);
	//Style Drop Down list items
	lv_obj_t* list = lv_dropdown_get_list(dropDown);
	lv_obj_remove_style_all(list);
	lv_obj_add_style(list, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(list, &mainStyleSelected, LV_PART_SELECTED);
	lv_obj_set_style_text_line_space(list, 6, LV_PART_MAIN);
	//Add event
	lv_obj_add_event_cb(dropDown, GUIMenuDropDownEvent, LV_EVENT_VALUE_CHANGED, NULL);

	return cont;
}

lv_obj_t* GUIMenuCreateTextArea(lv_obj_t* parent, char* text, uint8_t maxChars, char* defaultText) {
	lv_obj_t* cont = lv_menu_cont_create(parent);
	lv_obj_remove_style_all(cont);
	lv_obj_add_style(cont, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(cont, &mainStylePressed, LV_PART_MAIN | LV_STATE_CHECKED);
	lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
//	lv_obj_set_style_bg_color(cont, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_set_style_pad_all(cont, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_left(cont, 2, LV_PART_MAIN);
	lv_obj_set_height(cont, 30);
	lv_obj_t* label = lv_label_create(cont);
	lv_label_set_text(label, text);
	lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);

	//Text Area
	lv_obj_t* textArea = lv_textarea_create(cont);
	lv_obj_remove_style_all(textArea);
	lv_obj_add_style(textArea, &mainStyle, LV_PART_MAIN);
	lv_textarea_set_one_line(textArea, true);
	lv_textarea_set_max_length(textArea, maxChars);
	lv_obj_set_style_text_align(textArea, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
//	lv_obj_set_style_border_width(textArea, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(textArea, 2, LV_PART_CURSOR | LV_STATE_FOCUSED);
	lv_obj_set_style_border_side(textArea, LV_BORDER_SIDE_LEFT, LV_PART_CURSOR);
	lv_obj_set_style_border_color(textArea, STYLE_COLOR_GREY, LV_PART_CURSOR);
	lv_obj_set_size(textArea, 100, 30);
	lv_obj_align(textArea, LV_ALIGN_RIGHT_MID, 0, 0);
	lv_textarea_set_text(textArea, defaultText);
	lv_obj_add_event_cb(textArea, GUIMenuTextArea, LV_EVENT_FOCUSED, numPad);
	lv_obj_add_event_cb(textArea, GUIMenuTextArea, LV_EVENT_DEFOCUSED, numPad);
	lv_obj_add_event_cb(textArea, GUIMenuTextArea, LV_EVENT_READY, numPad);
}
