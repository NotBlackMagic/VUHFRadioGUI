#include "guiNumPad.h"

//Helper Objects
lv_obj_t* numPad;

/**
  * @brief	This function initializes this GUI part
  * @param	None
  * @return	None
  */
void GUINumPadInit() {
	//Costume NumPad Map
	static const char * kb_map[] = {"7", "8", "9", "\n",
									"4", "5", "6", "\n",
									"1", "2", "3", "\n",
									LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_OK, NULL};

	static const lv_btnmatrix_ctrl_t kb_ctrl[] = {	1, 1, 1,
													1, 1, 1,
													1, 1, 1,
													1, 1, 1};

	/*Create a keyboard to use it with an of the text areas*/
	numPad = lv_keyboard_create(lv_scr_act());
	lv_obj_remove_style_all(numPad);
	lv_obj_add_style(numPad, &mainStyle, LV_PART_MAIN);
	lv_obj_add_style(numPad, &mainStyle, LV_PART_ITEMS);
	lv_obj_add_style(numPad, &mainStylePressed, LV_PART_ITEMS | LV_STATE_PRESSED);
	lv_obj_align(numPad, LV_ALIGN_BOTTOM_MID, 0, -10);
	//Correct padding for row and column spacing (applied only to one side...)
	lv_obj_set_style_pad_right(numPad, 2, LV_PART_MAIN);
	lv_obj_set_style_pad_bottom(numPad, 2, LV_PART_MAIN);
	lv_obj_set_size(numPad, 120, 160);
	lv_keyboard_set_map(numPad, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
	lv_keyboard_set_mode(numPad, LV_KEYBOARD_MODE_USER_1);

	lv_keyboard_set_textarea(numPad, NULL);
	lv_obj_add_flag(numPad, LV_OBJ_FLAG_HIDDEN);
}

//lv_obj_t * win;
//static void WindowCloseButtonEvent(lv_event_t * e) {
//	lv_obj_add_flag(win, LV_OBJ_FLAG_HIDDEN);
//}
//
//void GUIInitKeyPad() {
//	win = lv_win_create(lv_scr_act(), 30);
//	lv_win_add_title(win, "A title");
//	lv_obj_set_style_bg_color(win, lv_color_hex(0x000000), LV_PART_MAIN);
//	lv_obj_set_style_pad_all(win, 0, LV_PART_MAIN);
//	lv_obj_set_style_border_width(win, 2, LV_PART_MAIN);
//	lv_obj_set_style_border_color(win, lv_color_hex(0x404040), LV_PART_MAIN);
//	lv_obj_set_size(win, 200, 250);
//	lv_obj_align(win, LV_ALIGN_CENTER, 0, 0);
//
//	lv_obj_t* btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 60);
//	lv_obj_remove_style_all(btn);
//	lv_obj_add_style(btn, &mainStyle, 0);
//	lv_obj_add_style(btn, &mainStylePressed, LV_STATE_PRESSED);
//	lv_obj_set_width(btn, 30);
//	lv_obj_add_event_cb(btn, WindowCloseButtonEvent, LV_EVENT_CLICKED, NULL);
//
//	lv_obj_t * head = lv_win_get_header(win);
//	lv_obj_set_style_bg_color(head, lv_color_hex(0x404040), LV_PART_MAIN);
//
//	lv_obj_t * cont = lv_win_get_content(win);
//	lv_obj_set_style_bg_color(cont, lv_color_hex(0x000000), LV_PART_MAIN);
//	lv_obj_set_style_pad_all(cont, 4, LV_PART_MAIN);
//
//	/*Create a text area. The keyboard will write here*/
//	lv_obj_t * ta;
//	ta = lv_textarea_create(cont);
//	lv_obj_remove_style_all(ta);
//	lv_obj_add_style(ta, &mainStyle, LV_PART_MAIN);
//	lv_textarea_set_one_line(ta, true);
//	lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 0);
//	lv_obj_set_width(ta, lv_pct(90));
//	lv_obj_add_state(ta, LV_STATE_FOCUSED);
//
//	lv_keyboard_set_textarea(numPad, ta);
//}
