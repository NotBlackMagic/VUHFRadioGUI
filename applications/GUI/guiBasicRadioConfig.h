#ifndef APPLICATIONS_GUI_GUIBASICRADIOCONFIG_H_
#define APPLICATIONS_GUI_GUIBASICRADIOCONFIG_H_

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

//GUI Parts
#include "guiTracking.h"

//Radio Basic Configuration GUI Objects
extern lv_obj_t* radioModeDropDown;
extern lv_obj_t* radioModulationDropDown;
extern lv_obj_t* radioAGCDropDown;
extern lv_obj_t* radioAFCDropDown;
extern lv_obj_t* radioAFCEnableButton;

void GUIBasicRadioConfigAreaInit();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUIBASICRADIOCONFIG_H_ */
