#include "customWidgets.h"

lv_obj_t* CustomScaleCreateParent(lv_obj_t* parent, lv_coord_t padX, lv_coord_t padY, lv_coord_t lineWidth, lv_coord_t lineHeight, int16_t ticks, bool invert) {
	lv_obj_update_layout(parent);				//Force update object layout calculations
	lv_coord_t w = lv_obj_get_width(parent);
	lv_coord_t h = lv_obj_get_height(parent);

	//Increase offset by half line dimension
//	padX += lineWidth / 2;

	uint16_t nPoints = 4 + ticks * 3;
	lv_point_t *points = lv_mem_alloc(sizeof(lv_point_t)*nPoints);

	//Set start and end tick line
	lv_coord_t x = padX;
	lv_coord_t y = h - (padY + lineHeight);
	if(invert == true) {
		y = h - padY;
	}
	points[0] = (lv_point_t){x, y};
	y = h - padY;
	if(invert == true) {
		y = h - (padY + lineHeight);
	}
	points[1] = (lv_point_t){x, y};
	x = w - padX;
	points[nPoints - 2] = (lv_point_t){x, y};
	y = h - (padY + lineHeight);
	if(invert == true) {
		y = h - padY;
	}
	points[nPoints - 1] = (lv_point_t){x, y};

	//Calculate scale line tick points
	uint16_t i;
	for(i = 0; i < ticks; i++) {
		x = lv_map(i + 1, 0, (ticks + 1), padX, w - padX);
		//Set tick line bottom
		y = h - padY;
		if(invert == true) {
			y = h - (padY + lineHeight);
		}
		points[2 + i*3] = (lv_point_t){x, y};
		//Set tick line top
		y = h - (padY + lineHeight);
		if(invert == true) {
			y = h - padY;
		}
		points[2 + i*3 + 1] = (lv_point_t){x, y};
		//Set tick line bottom, again
		y = h - padY;
		if(invert == true) {
			y = h - (padY + lineHeight);
		}
		points[2 + i*3 + 2] = (lv_point_t){x, y};
	}

	//lv_obj_t * line = lv_line_create(parent);
	lv_obj_t * line = lv_line_create(parent);
	lv_line_set_points(line, points, nPoints);
	lv_obj_remove_style_all(line);
	lv_obj_add_style(line, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(line, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(line, 0, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(line, LV_OPA_0, LV_PART_MAIN);
	lv_obj_set_style_line_width(line, lineWidth, LV_PART_MAIN);

	return line;
}

lv_obj_t* CustomScaleCreate(lv_coord_t width, lv_coord_t height, lv_coord_t posX, lv_coord_t poxY, lv_coord_t lineWidth, int16_t ticks, bool invert) {
	//lv_obj_update_layout(parent);				//Force update object layout calculations

	//Increase offset by half line dimension
	posX += lineWidth / 2;

	uint16_t nPoints = 4 + ticks * 3;
	lv_point_t *points = lv_mem_alloc(sizeof(lv_point_t)*nPoints);

	//Set start and end tick line
	lv_coord_t x = posX;
	lv_coord_t y = poxY;
	if(invert == true) {
		y = poxY + height;
	}
	points[0] = (lv_point_t){x, y};
	y = poxY + height;
	if(invert == true) {
		y = poxY;
	}
	points[1] = (lv_point_t){x, y};
	x = posX + width;
	points[nPoints - 2] = (lv_point_t){x, y};
	y = poxY;
	if(invert == true) {
		y = poxY + height;
	}
	points[nPoints - 1] = (lv_point_t){x, y};

	//Calculate scale line tick points
	uint16_t i;
	for(i = 0; i < ticks; i++) {
		x = lv_map(i + 1, 0, (ticks + 1), posX, posX + width);
		//Set tick line bottom
		y = poxY + height;
		if(invert == true) {
			y = poxY;
		}
		points[2 + i*3] = (lv_point_t){x, y};
		//Set tick line top
		y = poxY;
		if(invert == true) {
			y = poxY + height;
		}
		points[2 + i*3 + 1] = (lv_point_t){x, y};
		//Set tick line bottom, again
		y = poxY + height;
		if(invert == true) {
			y = poxY;
		}
		points[2 + i*3 + 2] = (lv_point_t){x, y};
	}

	//lv_obj_t * line = lv_line_create(parent);
	lv_obj_t * line = lv_line_create(lv_scr_act());
	lv_line_set_points(line, points, nPoints);
	lv_obj_remove_style_all(line);
	lv_obj_add_style(line, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(line, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(line, 0, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(line, LV_OPA_0, LV_PART_MAIN);
	lv_obj_set_style_line_width(line, lineWidth, LV_PART_MAIN);

	return line;
}
