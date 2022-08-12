#ifndef APPLICATIONS_GUI_GUI_H_
#define APPLICATIONS_GUI_GUI_H_

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
#include "guiBasicRadioConfig.h"
#include "guiBottomBar.h"
#include "guiCF.h"
#include "guiMain.h"
#include "guiNumPad.h"
#include "guiTopBar.h"
#include "guiTracking.h"

void GUIInit();
void GUIUpdate();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUI_H_ */
