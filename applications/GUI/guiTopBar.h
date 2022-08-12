#ifndef APPLICATIONS_GUI_GUITOPBAR_H_
#define APPLICATIONS_GUI_GUITOPBAR_H_

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

//Top bar GUI objects
extern lv_obj_t* topBarArea;
extern lv_obj_t* batteryLabel;
extern lv_obj_t* timeLabel;
extern lv_obj_t* powerModeLabel;

void GUITopBarInit();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUITOPBAR_H_ */
