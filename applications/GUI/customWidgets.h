#ifndef APPLICATIONS_GUI_CUSTOMWIDGETS_H_
#define APPLICATIONS_GUI_CUSTOMWIDGETS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "styles.h"

lv_obj_t* CustomScaleCreateParent(lv_obj_t* parent, lv_coord_t offsetX, lv_coord_t offsetY, lv_coord_t lineWidth, lv_coord_t lineHeight, int16_t ticks, bool invert);
lv_obj_t* CustomScaleCreate(lv_coord_t width, lv_coord_t height, lv_coord_t posX, lv_coord_t poxY, lv_coord_t lineWidth, int16_t ticks, bool invert);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_CUSTOMWIDGETS_H_ */
