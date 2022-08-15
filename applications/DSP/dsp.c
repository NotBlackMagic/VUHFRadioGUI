#include "dsp.h"

static struct rt_thread dspThread;
static rt_uint8_t dspThreadStack[DSP_THREAD_STACK_SIZE];

const arm_cfft_instance_f32* cfftInstanceF32 = &arm_cfft_sR_f32_len16;

int16_t iqValuesQ15[32];
float iqValuesF32[32];
void DSPThread() {
	while(1) {
		uint8_t i;
		for(i = 0; i < 16; i++) {
			uint16_t sinPh = (2 * (INT16_MAX / 16) * i) & INT16_MAX;
			uint16_t cosPh = ((2 * (INT16_MAX / 16) * i) + INT16_MAX / 4) & INT16_MAX;
			iqValuesQ15[2*i] = arm_sin_q15(cosPh);		//Real (I) part
			iqValuesQ15[2*i + 1] = 0;//arm_sin_q15(sinPh);	//Imaginary (Q) part
		}

		//Convert from Q15 to float
		arm_q15_to_float(iqValuesQ15, iqValuesF32, 32);

		//Calculate complex FFT of IQ values
		arm_cfft_f32(cfftInstanceF32, iqValuesF32, 0, 1);
		arm_abs_f32(iqValuesF32, iqValuesF32, 32);
		for(i = 0; i < 32; i++) {
			iqValuesF32[i] = 20.0f * log10f(iqValuesF32[i] + 0.00001f);		//Add some value to avoid log(0)
		}

		//Re-organize FFT output from [DC -> N/2 -> -DC] to [-N/2 -> DC -> N/2]
		float mid = iqValuesF32[15];
		for(i = 0; i < 15; i++) {
			float tmp = iqValuesF32[i];
			iqValuesF32[i] = iqValuesF32[17 + i];
			iqValuesF32[15 + i] = tmp;
		}
		iqValuesF32[30] = mid;

		//Convert from float to int16
		for(i = 0; i < 32; i++) {
			iqValuesQ15[i] = (int16_t)iqValuesF32[i];
		}

		//Send calculated FFT to GUI
		InterThreadMessageStruct guiMsg = {.id = InterThread_TrackingSpectrum, .data = (uint32_t*)iqValuesQ15, .length = 31 };
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
