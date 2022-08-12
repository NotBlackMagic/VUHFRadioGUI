#ifndef APPLICATIONS_GUI_GUITRACKING_H_
#define APPLICATIONS_GUI_GUITRACKING_H_

#ifdef __cplusplus
extern "C" {
#endif

//General C libraries includes
#include "string.h"

//LVGL and RT-Thread include
#include "lvgl.h"
#include "rtdevice.h"

//RT-Thread Driver includes
#include "drv_gpio.h"

//Custom application includes
#include "customWidgets.h"
#include "interThreadCommunication.h"
#include "styles.h"

//Radio Tracking Information GUI objects
extern lv_obj_t* rssiTrackingArea;
extern lv_obj_t* rssiTrackingBar;
extern lv_obj_t* rfTrackingArea;
extern lv_obj_t* rfTrackingBar;
extern lv_obj_t* rfAFCRangeBar;
extern lv_obj_t* emptyTrackingArea;

void GUITrackingAreaInit();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUITRACKING_H_ */
