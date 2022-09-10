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
	GUIMenuInit();
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
			case InterThread_TNCPacket: {
				AX25Struct* ax25Packet = (AX25Struct*)(msg.data);
				GUIRMainAreaMessageUpdate(ax25Packet);
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
//				lv_dropdown_set_selected(radioModulationDropDown, modulation);
				break;
			}
			case InterThread_AGCSpeed: {
				uint32_t mode = (uint32_t)(msg.data);
				//Convert to AGC Drop Down index
				uint8_t agcSpeedToList[16] = {0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4, 4, 5};
				lv_dropdown_set_selected(radioAGCDropDown, agcSpeedToList[mode]);
				break;
			}
			case InterThread_AFC: {
				uint32_t afc = (uint32_t)(msg.data);

				uint8_t afcRatio = 0;
				if(afc > 0) {
					afcRatio = radioAConfig.bandwidth / afc;
				}

				uint8_t selected = 0;
				if(afcRatio < 3) {
					//AFC Disabled
					selected = 3;
				}
				else if(afcRatio < 6) {
					//AFC Range Wide: 1/4 of bandwidth
					selected = 0;
				}
				else if(afcRatio < 12) {
					//AFC Range Middle: 1/8 of bandwidth
					selected = 1;
				}
				else if(afcRatio < 24) {
					//AFC Range Narrow: 1/8 of bandwidth
					selected = 1;
				}
				else {
					//AFC Disabled
					selected = 3;
				}
				lv_dropdown_set_selected(radioAFCDropDown, selected);

				//Change AFC Bar
				switch(selected) {
					case 0x00: {
						//AFC Range Wide: 1/4 of bandwidth
						//Set Size
						static lv_point_t rfTrackingBandwidthPoints[] = { {0, 0}, {90, 0} };
						lv_line_set_points(rfAFCRangeBar, rfTrackingBandwidthPoints, 2);
						lv_obj_set_pos(rfAFCRangeBar, (210 - 90) / 2, 7);
						//Enable Bar
						lv_obj_clear_flag(rfAFCRangeBar, LV_OBJ_FLAG_HIDDEN);
						break;
					}
					case 0x01: {
						//AFC Range Middle: 1/8 of bandwidth
						//Set Size
						static lv_point_t rfTrackingBandwidthPoints[] = { {0, 0}, {45, 0} };
						lv_line_set_points(rfAFCRangeBar, rfTrackingBandwidthPoints, 2);
						lv_obj_set_pos(rfAFCRangeBar, (210 - 45) / 2, 7);
						//Enable Bar
						lv_obj_clear_flag(rfAFCRangeBar, LV_OBJ_FLAG_HIDDEN);
						break;
					}
					case 0x02: {
						//AFC Range Narrow: 1/16 of bandwidth
						//Set Size
						static lv_point_t rfTrackingBandwidthPoints[] = { {0, 0}, {23, 0} };
						lv_line_set_points(rfAFCRangeBar, rfTrackingBandwidthPoints, 2);
						lv_obj_set_pos(rfAFCRangeBar, (210 - 23) / 2, 7);
						//Enable Bar
						lv_obj_clear_flag(rfAFCRangeBar, LV_OBJ_FLAG_HIDDEN);
						break;
					}
					case 0x03: {
						//AFC Disabled
						lv_obj_add_flag(rfAFCRangeBar, LV_OBJ_FLAG_HIDDEN);
						break;
					}
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
			case InterThread_DatarateRX: {
				uint32_t datarate = (uint32_t)(msg.data);
				char str[12];
				sprintf(str, "%d", datarate);
//				lv_textarea_set_text(radioRXDatarateTextArea, str);
			}
			case InterThread_Encoding: {
				uint32_t encoding = (uint32_t)(msg.data);
//				lv_dropdown_set_selected(radioPacketEncodingDropDown, encoding);
				break;
			}
			case InterThread_Framing: {
				uint32_t framing = (uint32_t)(msg.data);
//				lv_dropdown_set_selected(radioPacketFramingDropDown, framing);
				break;
			}
			case InterThread_CRC: {
				uint32_t crc = (uint32_t)(msg.data);
//				lv_dropdown_set_selected(radioPacketCRCDropDown, crc);
				break;
			}
			case InterThread_TNCEnable: {
				uint32_t tnc = (uint32_t)(msg.data);
				if(tnc != 0) {
					lv_obj_add_state(radioTNCEnableButton, LV_STATE_CHECKED);
				}
				else {
					lv_obj_clear_state(radioTNCEnableButton, LV_STATE_CHECKED);
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
			case InterThread_TrackingAGCGain: {
				uint8_t gain = (uint8_t)(msg.data);
				lv_bar_set_value(agcGainTrackingBar, gain, LV_ANIM_OFF);
				break;
			}
			case InterThread_TrackingSpectrum: {
				if(lv_obj_has_flag(spectrumChart, LV_OBJ_FLAG_HIDDEN) == true) {
					//This chart is hidden so ignore
					break;
				}

				//Add points
				uint16_t chartSize = lv_chart_get_point_count(spectrumChart);
				if(chartSize != msg.length) {
					break;
				}

				lv_chart_series_t * ser = lv_chart_get_series_next(spectrumChart, NULL);

			    uint16_t i;
			    for(i = 0; i < chartSize; i++) {
			    	ser->y_points[i] = ((int16_t*)msg.data)[i];
			    }
			    lv_chart_refresh(spectrumChart);
				break;
			}
			case InterThread_TrackingSignal: {
				if(lv_obj_has_flag(signalChart, LV_OBJ_FLAG_HIDDEN) == true) {
					//This chart is hidden so ignore
					break;
				}

				//Add points
				uint16_t chartSize = lv_chart_get_point_count(signalChart);
				if(chartSize != (msg.length / 2)) {
					break;
				}

				lv_chart_series_t * serI = lv_chart_get_series_next(signalChart, NULL);
				lv_chart_series_t * serQ = lv_chart_get_series_next(signalChart, serI);

				uint16_t i;
				for(i = 0; i < chartSize; i++) {
					//I Signal
					serI->y_points[i] = ((int16_t*)msg.data)[2*i] >> 8;
					//Q Signal
					serQ->y_points[i] = ((int16_t*)msg.data)[2*i + 1] >> 8;
				}
				lv_chart_refresh(signalChart);
				break;
			}
		}

	}
}
