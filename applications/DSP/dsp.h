#ifndef APPLICATIONS_DSP_DSP_H_
#define APPLICATIONS_DSP_DSP_H_

#ifdef __cplusplus
extern "C" {
#endif

//General C libraries includes
#include "string.h"

//ARM Math (CMSIS-DSP)
#include "arm_math.h"
#include "arm_const_structs.h"

//LVGL and RT-Thread include
#include "lvgl.h"
#include "rtdevice.h"

//RT-Thread Driver includes
#include "drv_gpio.h"

//Custom application includes
#include "interThreadCommunication.h"

//DSP Parts

void DSPInit();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_DSP_DSP_H_ */
