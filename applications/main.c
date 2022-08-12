#include "MIMXRT1062.h"
#include <rtdevice.h>
#include "drv_gpio.h"
#include "core_cm7.h"

//Defined the LED pin: GPIO1_IO9
#define LED0_PIN               GET_PIN(1, 8)

int main(void) {
	//Set LED0 pin mode to output
	rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

	while(1) {
		rt_pin_write(LED0_PIN, PIN_HIGH);
		rt_thread_mdelay(5000);
		rt_pin_write(LED0_PIN, PIN_LOW);
		rt_thread_mdelay(5000);
	}
}

void reboot(void) {
	NVIC_SystemReset();
}
MSH_CMD_EXPORT(reboot, reset system)
