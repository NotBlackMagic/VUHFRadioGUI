#include "guiInfoWindow.h"

#define GUI_HARDWARE_VERSION_MAJOR				1
#define GUI_HARDWARE_VERSION_MINOR				0
#define GUI_FIRMWARE_VERSION_MAJOR				1
#define GUI_FIRMWARE_VERSION_MINOR				10
#define GUI_FIRMWARE_VERSION_HASH				0xAAAAAAAA

//Radio Information Window GUI Objects
lv_obj_t* infoWindow;
lv_obj_t* infoLabel;

/**
  * @brief	This function handles button events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
static void GUIRadioInfoWindowButtonEvent(lv_event_t * e) {
	lv_obj_add_flag(infoWindow, LV_OBJ_FLAG_HIDDEN);
}

/**
  * @brief	This function is used to update more complex GUI elements in this GUI part
  * @param	None
  * @return	None
  */
void GUIRadioInfoWindowUpdate() {
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

	lv_label_set_text(infoLabel, str);
}

/**
  * @brief	This function initializes this GUI part
  * @param	None
  * @return	None
  */
void GUIRadioInfoWindowInit() {
	infoWindow = lv_win_create(lv_scr_act(), 30);
	lv_win_add_title(infoWindow, "Information");
	lv_obj_set_style_bg_color(infoWindow, STYLE_COLOR_BLACK, LV_PART_MAIN);
	lv_obj_set_style_pad_all(infoWindow, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(infoWindow, 2, LV_PART_MAIN);
	lv_obj_set_style_border_color(infoWindow, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_size(infoWindow, 200, 230);
	lv_obj_align(infoWindow, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t* btn = lv_win_add_btn(infoWindow, LV_SYMBOL_CLOSE, 60);
	lv_obj_remove_style_all(btn);
	lv_obj_add_style(btn, &mainStyle, 0);
	lv_obj_add_style(btn, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_width(btn, 30);
	lv_obj_add_event_cb(btn, GUIRadioInfoWindowButtonEvent, LV_EVENT_CLICKED, NULL);

	lv_obj_t * head = lv_win_get_header(infoWindow);
//	lv_obj_remove_style_all(head);
//	lv_obj_add_style(head, &mainStyle, 0);
	lv_obj_set_style_bg_color(head, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_text_color(head, STYLE_COLOR_WHITE, LV_PART_MAIN);

	lv_obj_t * cont = lv_win_get_content(infoWindow);
	lv_obj_set_style_bg_color(cont, STYLE_COLOR_BLACK, LV_PART_MAIN);
	lv_obj_set_style_pad_all(cont, 4, LV_PART_MAIN);

	//Create the Information Label
	infoLabel = lv_label_create(cont);
	lv_obj_remove_style_all(infoLabel);
	lv_obj_add_style(infoLabel, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(infoLabel, 0, LV_PART_MAIN);
//	lv_obj_set_style_border_color(infoLabel, STYLE_COLOR_GREY, LV_PART_MAIN);
//	lv_obj_set_style_text_color(infoLabel, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_align(infoLabel, LV_ALIGN_TOP_LEFT, 0, 0);
	lv_label_set_text(infoLabel, "NONE");

	lv_obj_add_flag(infoWindow, LV_OBJ_FLAG_HIDDEN);
}
