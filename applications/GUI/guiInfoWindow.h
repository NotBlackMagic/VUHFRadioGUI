#ifndef APPLICATIONS_GUI_GUIINFOWINDOW_H_
#define APPLICATIONS_GUI_GUIINFOWINDOW_H_

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
#include "radioConfigs.h"
#include "styles.h"

//GUI Parts

//Radio information window GUI objects
extern lv_obj_t * infoWindow;
extern lv_obj_t* infoLabel;

void GUIRadioInfoWindowInit();
void GUIRadioInfoWindowUpdate();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUIINFOWINDOW_H_ */
