#ifndef APPLICATIONS_GUI_GUIMAIN_H_
#define APPLICATIONS_GUI_GUIMAIN_H_

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
#include "ax25.h"
#include "customWidgets.h"
#include "interThreadCommunication.h"
#include "styles.h"

//Main Area GUI Objects
extern lv_obj_t* messageArea;
extern lv_obj_t* spectrumChart;

void GUIMainAreaInit();
void GUIRMainAreaMessageUpdate(AX25Struct* packet);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUIMAIN_H_ */
