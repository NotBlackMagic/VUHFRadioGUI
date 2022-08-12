#ifndef APPLICATIONS_GUI_GUICF_H_
#define APPLICATIONS_GUI_GUICF_H_

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
#include "guiNumPad.h"
#include "guiTracking.h"

//Radio Configuration GUI objects
extern lv_obj_t* centerFrequencyArea;
extern lv_obj_t* centerFrequencyTextArea;
extern lv_obj_t* centerFrequencyLockLabel;
extern lv_obj_t* centerFrequencyBandLabel;
extern lv_obj_t* bandwidthTextArea;
extern lv_obj_t* afcRangeLabel;

void GUICFAreaInit();
void GUIRadioBandwidthUpdate();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUICF_H_ */
