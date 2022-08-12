#include "gui.h"

struct rt_messagequeue guiMessageQueue;
static uint8_t guiMessagePool[GUI_MESSAGE_POOL_SIZE];

/**
  * @brief	This function handles the initialization of the user GUI interface, called from the GUI Thread Initialization function
  * @param	None
  * @return	None
  */
void lv_user_gui_init(void) {
	StylesInit();
    GUIInit();
}

/**
  * @brief	This function handles updates to the GUI, called from the GUI Thread function before each LVGL update call (lv_task_handler).
  * @param	None
  * @return	None
  */
void lv_user_gui_update(void) {
	GUIUpdate();
}

/**
  * @brief	This function initializes the GUI interface
  * @param	None
  * @return	None
  */
void GUIInit() {
	//Initialize the GUI message queue
	rt_err_t status = rt_mq_init(	&guiMessageQueue,
									"GUIMsgQu",
									&guiMessagePool[0],
									sizeof(InterThreadMessageStruct),
									sizeof(guiMessagePool),
									RT_IPC_FLAG_FIFO);

	//Set screen background color
	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0);

	//Initialize each part of the GUI interface
	GUINumPadInit();
	GUITopBarInit();
	GUICFAreaInit();
	GUITrackingAreaInit();
	GUIBasicRadioConfigAreaInit();
	GUIMainAreaInit();
	GUIBottomBarInit();
	GUIRadioInfoWindowInit();
}

/**
  * @brief	This function updates the GUI interface
  * @param	None
  * @return	None
  */
void GUIUpdate() {
	//Update Time
	time_t now = time(RT_NULL);
	struct tm tm = *localtime(&now);
	char str[12];
	sprintf(str, "%02d:%02d", tm.tm_hour, tm.tm_min);
	lv_label_set_text(timeLabel, str);

	//Check if new message in the thread queue, used to transmit necessary updates to the GUI. None blocking call to keep the GUI responsive!
	InterThreadMessageStruct msg;
	if(rt_mq_recv(&guiMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct), RT_WAITING_NO) == RT_EOK) {
		switch(msg.id) {
			case 0x00: {
//				lv_label_set_text(label1, msg.data);
				break;
			}
			case InterThread_CenterFrequency: {
				uint32_t freq = (uint32_t)(msg.data);
				char str[12];
				sprintf(str, "%d", freq);
				lv_textarea_set_text(centerFrequencyTextArea, str);
				break;
			}
			case InterThread_CFLock: {
				uint8_t cfLock = (uint8_t)(msg.data);
				if(cfLock == 0x01) {
					lv_obj_set_style_text_color(centerFrequencyLockLabel, STYLE_COLOR_WHITE, LV_PART_MAIN);
				}
				else {
					lv_obj_set_style_text_color(centerFrequencyLockLabel, STYLE_COLOR_GREY, LV_PART_MAIN);
				}

				break;
			}
			case InterThread_Bandwidth: {
				uint32_t freq = (uint32_t)(msg.data);
				char str[12];
				sprintf(str, "%d", freq);
				lv_textarea_set_text(bandwidthTextArea, str);
				GUIRadioBandwidthUpdate();
				break;
			}
			case InterThread_Modulation: {
				uint32_t modulation = (uint32_t)(msg.data);
				lv_dropdown_set_selected(radioModulationDropDown, modulation);
				break;
			}
			case InterThread_AGCSpeed: {
				uint32_t mode = (uint32_t)(msg.data);
				lv_dropdown_set_selected(radioAGCDropDown, mode);
				break;
			}
			case InterThread_AFC: {
				uint32_t afc = (uint32_t)(msg.data);
				if(afc != 0) {
					lv_obj_add_state(radioAFCEnableButton, LV_STATE_CHECKED);
				}
				else {
					lv_obj_clear_state(radioAFCEnableButton, LV_STATE_CHECKED);
				}
				break;
			}
			case InterThread_Mode: {
				uint32_t mode = (uint32_t)(msg.data);
				lv_dropdown_set_selected(radioModeDropDown, mode);
				if(mode == 0) {
					lv_obj_set_style_border_color(radioModeDropDown, STYLE_COLOR_DARK_RED, LV_PART_MAIN);
					lv_obj_set_style_text_color(radioModeDropDown, STYLE_COLOR_RED, LV_PART_MAIN);
				}
				else {
					lv_obj_set_style_border_color(radioModeDropDown, STYLE_COLOR_DARK_GREEN, LV_PART_MAIN);
					lv_obj_set_style_text_color(radioModeDropDown, STYLE_COLOR_GREEN, LV_PART_MAIN);
				}
				break;
			}
			case InterThread_TrackingRSSI: {
				int32_t rssi = (int32_t)(msg.data);
				lv_bar_set_value(rssiTrackingBar, 128 + rssi, LV_ANIM_OFF);
				break;
			}
			case InterThread_TrackingRFOffset: {
				int32_t freq = (int32_t)(msg.data);
				lv_coord_t x = lv_map(freq, -16000, 16000, 16, 280 - 16);
				lv_obj_set_pos(rfTrackingBar, x, 2);
				break;
			}
			case InterThread_TrackingSpectrum: {
			    //Add points
				uint8_t chartSize = lv_chart_get_point_count(spectrumChart);
				if(chartSize != msg.length) {
					break;
				}

				lv_chart_series_t * ser = lv_chart_get_series_next(spectrumChart, NULL);

			    uint8_t i;
			    for(i = 0; i < chartSize; i++) {
			    	ser->y_points[i] = ((int16_t*)msg.data)[i];
			    }
			    lv_chart_refresh(spectrumChart);
				break;
			}
		}

	}
}
