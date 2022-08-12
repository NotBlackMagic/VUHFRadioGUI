#include "dsp.h"

static struct rt_thread dspThread;
static rt_uint8_t dspThreadStack[DSP_THREAD_STACK_SIZE];

const arm_cfft_instance_f32* cfftInstanceF32 = &arm_cfft_sR_f32_len16;

int16_t iqValuesQ15[32];
float iqValuesF32[32];
void DSPThread() {
	while(1) {
		uint8_t i;
		for(i = 0; i < 32; i++) {
			iqValuesQ15[i] = (INT16_MAX - lv_rand(0, UINT16_MAX));
			iqValuesF32[i] = (float)iqValuesQ15[i] / INT16_MAX;
		}

		//Calculate complex FFT of IQ values
		arm_cfft_f32(cfftInstanceF32, iqValuesF32, 0, 1);
		arm_abs_f32(iqValuesF32, iqValuesF32, 32);
		for(i = 0; i < 32; i++) {
			iqValuesF32[i] = 20 * log10f(iqValuesF32[i]);
			iqValuesQ15[i] = (int16_t)iqValuesF32[i];
		}

		//Send calculated FFT to GUI
		InterThreadMessageStruct guiMsg = {.id = InterThread_TrackingSpectrum, .data = (uint32_t*)iqValuesQ15, .length = 32 };
		rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));

		rt_thread_mdelay(1000);
	}
}

void DSPThreadInit() {
	rt_err_t status;

	status = rt_thread_init(&dspThread,
							"dspTh",
							DSPThread,
							RT_NULL,
							&dspThreadStack[0],
							sizeof(dspThreadStack),
							DSP_THREAD_PRIORITY,
							10);

	status = rt_thread_startup(&dspThread);

	return 0;
}
INIT_ENV_EXPORT(DSPThreadInit);
