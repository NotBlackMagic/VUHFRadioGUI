#include "guiBottomBar.h"

//Bottom bar GUI objects
lv_obj_t* bottomBarArea;
lv_obj_t* menuButton;
lv_obj_t* screenSelectionDropDown;
lv_obj_t* infoButton;

/**
  * @brief	This function handles button events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
static void GUIBottomBarButtonEvent(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t * btn = lv_event_get_target(e);
	if(code == LV_EVENT_CLICKED) {
		if(btn == menuButton) {
			if(lv_obj_has_flag(menu, LV_OBJ_FLAG_HIDDEN) == true) {
				//Menu is hidden (closed) so open it
				lv_obj_clear_flag(menu, LV_OBJ_FLAG_HIDDEN);
			}
			else {
				//Menu is not hidden (open) so close it
				lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
			}
		}
		else if(btn == infoButton) {
			GUIRadioInfoWindowUpdate();

			lv_obj_clear_flag(infoWindow, LV_OBJ_FLAG_HIDDEN);
		}
	}
}

/**
  * @brief	This function handles Drop Down events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
static void GUIBottomBarDropDownEvent(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t * obj = lv_event_get_target(e);
	if(code == LV_EVENT_VALUE_CHANGED) {
		if(obj == screenSelectionDropDown) {
			uint16_t selected = lv_dropdown_get_selected(obj);
			switch(selected) {
				case 0:
					//Selected Message View
					lv_obj_clear_flag(messageArea, LV_OBJ_FLAG_HIDDEN);
					lv_obj_add_flag(spectrumChart, LV_OBJ_FLAG_HIDDEN);
					lv_obj_add_flag(signalChart, LV_OBJ_FLAG_HIDDEN);
					break;
				case 1:
					//Selected Spectrum View
					lv_obj_clear_flag(spectrumChart, LV_OBJ_FLAG_HIDDEN);
					lv_obj_add_flag(messageArea, LV_OBJ_FLAG_HIDDEN);
					lv_obj_add_flag(signalChart, LV_OBJ_FLAG_HIDDEN);
					break;
				case 2:
					//Selected Signal View
					lv_obj_clear_flag(signalChart, LV_OBJ_FLAG_HIDDEN);
					lv_obj_add_flag(messageArea, LV_OBJ_FLAG_HIDDEN);
					lv_obj_add_flag(spectrumChart, LV_OBJ_FLAG_HIDDEN);
					break;
				default:
					break;
			}
		}
	}
}

/**
  * @brief	This function initializes this GUI part
  * @param	None
  * @return	None
  */
void GUIBottomBarInit() {
	//Set Bottom Menu Bar
	//Bottom Bar Area
	bottomBarArea = lv_obj_create(lv_scr_act());	//lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(bottomBarArea);
	lv_obj_add_style(bottomBarArea, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(bottomBarArea, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(bottomBarArea, 0, LV_PART_MAIN);
	lv_obj_set_size(bottomBarArea, 480, 30);
	lv_obj_align(bottomBarArea, LV_ALIGN_BOTTOM_MID, 0, 0);

	//Menu Button
	menuButton = lv_btn_create(bottomBarArea);
	lv_obj_remove_style_all(menuButton);
	lv_obj_add_style(menuButton, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(menuButton, lv_color_hex(0x606060), LV_PART_MAIN);
	lv_obj_add_style(menuButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_size(menuButton, 96, 30);
//	lv_obj_set_pos(menuButton, 0, 0);
	lv_obj_align(menuButton, LV_ALIGN_LEFT_MID, 0, 0);
	//Button Label
	lv_obj_t* label = lv_label_create(menuButton);
	lv_label_set_text(label, "MENU");
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_center(label);
	//Add event
	lv_obj_add_event_cb(menuButton, GUIBottomBarButtonEvent, LV_EVENT_CLICKED, NULL);

	//Main Screen Selection Drop Down
	screenSelectionDropDown = lv_dropdown_create(bottomBarArea);
	lv_dropdown_set_options(screenSelectionDropDown,	"MESSAGE\n"
														"SPECTRUM\n"
														"SIGNAL");
	lv_obj_remove_style_all(screenSelectionDropDown);
	lv_obj_add_style(screenSelectionDropDown, &mainStyle, 0);
	lv_obj_set_style_bg_color(screenSelectionDropDown, lv_color_hex(0x606060), LV_PART_MAIN);
	lv_dropdown_set_symbol(screenSelectionDropDown, NULL);
	lv_obj_set_size(screenSelectionDropDown, 96, 30);
//	lv_obj_set_pos(screenSelectionDropDown, 60, 105);
	lv_obj_align(screenSelectionDropDown, LV_ALIGN_LEFT_MID, 96, 0);
	//Style Drop Down list items
	lv_obj_t* list = lv_dropdown_get_list(screenSelectionDropDown);
	lv_obj_remove_style_all(list);
	lv_obj_add_style(list, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(list, &mainStyleSelected, LV_PART_SELECTED);
	lv_obj_set_style_text_line_space(list, 6, LV_PART_MAIN);
	//Add event
	lv_obj_add_event_cb(screenSelectionDropDown, GUIBottomBarDropDownEvent, LV_EVENT_VALUE_CHANGED, NULL);

	//Info Button
	infoButton = lv_btn_create(bottomBarArea);
	lv_obj_remove_style_all(infoButton);
	lv_obj_add_style(infoButton, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(infoButton, lv_color_hex(0x606060), LV_PART_MAIN);
	lv_obj_add_style(infoButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_size(infoButton, 96, 30);
//	lv_obj_set_pos(infoButton, 0, 0);
	lv_obj_align(infoButton, LV_ALIGN_RIGHT_MID, 0, 0);
	//Button Label
	label = lv_label_create(infoButton);
	lv_label_set_text(label, "INFO");
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_center(label);
	//Add event
	lv_obj_add_event_cb(infoButton, GUIBottomBarButtonEvent, LV_EVENT_CLICKED, NULL);
}
