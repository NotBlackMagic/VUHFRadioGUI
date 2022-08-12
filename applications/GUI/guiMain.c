#include "guiMain.h"

#define GUI_SPECTRUM_CHART_POINTS					32

//Main Area GUI Objects
lv_obj_t* messageArea;
lv_obj_t* spectrumChart;

/**
  * @brief	This function initializes this GUI part
  * @param	None
  * @return	None
  */
void GUIMainAreaInit() {
	//Message Area
	messageArea = lv_label_create(lv_scr_act());
	lv_obj_remove_style_all(messageArea);
	lv_obj_add_style(messageArea, &mainStyle, LV_PART_MAIN);
	lv_obj_set_size(messageArea, 480, 107);
	lv_obj_set_pos(messageArea, 0, 135);
//	lv_obj_add_flag(messageArea, LV_OBJ_FLAG_HIDDEN);

	//Spectrum Chart
	spectrumChart = lv_chart_create(lv_scr_act());
	lv_obj_remove_style_all(spectrumChart);
	lv_obj_add_style(spectrumChart, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_pad_all(spectrumChart, 0, LV_PART_MAIN);			//Chart division line styling: LV_PART_MAIN
	lv_obj_set_style_line_color(spectrumChart, lv_color_hex(0x202020), LV_PART_MAIN);
	lv_obj_set_style_line_width(spectrumChart, 1, LV_PART_MAIN);
	lv_obj_add_style(spectrumChart, &mainStyle, LV_PART_ITEMS);			//Chart line styling: LV_PART_ITEMS
	lv_obj_add_style(spectrumChart, &mainStyle, LV_PART_INDICATOR);		//Chart points styling: LV_PART_INDICATOR
	lv_obj_set_style_border_width(spectrumChart, 0, LV_PART_INDICATOR);
	lv_obj_set_style_width(spectrumChart, 0, LV_PART_INDICATOR);
	lv_obj_set_style_height(spectrumChart, 0, LV_PART_INDICATOR);
	lv_obj_set_style_radius(spectrumChart, 0, LV_PART_INDICATOR);
	lv_obj_set_size(spectrumChart, 480, 107);
	lv_obj_set_pos(spectrumChart, 0, 135);
	lv_chart_set_type(spectrumChart, LV_CHART_TYPE_LINE);
//	lv_chart_set_range(spectrumChart, LV_CHART_AXIS_PRIMARY_X, -1000, 1000);
	lv_chart_set_range(spectrumChart, LV_CHART_AXIS_PRIMARY_Y, -128, 128);
	//Add Data Series
    lv_chart_series_t * ser = lv_chart_add_series(spectrumChart, lv_color_hex(0x0000FF), LV_CHART_AXIS_PRIMARY_Y);
	lv_chart_set_point_count(spectrumChart, GUI_SPECTRUM_CHART_POINTS);
    //Set division lines
	lv_chart_set_div_line_count(spectrumChart, 5, 5);
//	lv_chart_set_axis_tick(spectrumChart, LV_CHART_AXIS_PRIMARY_X, 7, 5, 5, 5, true, 50);
//	lv_chart_set_axis_tick(spectrumChart, LV_CHART_AXIS_PRIMARY_Y, 7, 5, 5, 1, true, 50);
	lv_obj_add_flag(messageArea, LV_OBJ_FLAG_HIDDEN);
}
