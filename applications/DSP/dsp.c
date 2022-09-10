#include "dsp.h"

#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_sai.h"
#include "fsl_sai_edma.h"

#define SAI_RX_BUFFER_SIZE					1024
#define SAI_TX_BUFFER_SIZE					1024

#define DSP_SIGNAL_LENGTH					256
#define DSP_FFT_LENGTH						512

bool bufferFull = false;
uint8_t saiBufferIndex = 0;

AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t txHandle);
edma_handle_t dmaTXHandle;
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t rxHandle);
edma_handle_t dmaRXHandle;
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t saiRXBuffer[SAI_RX_BUFFER_SIZE * 2], 4);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t saiTXBuffer[SAI_TX_BUFFER_SIZE * 2], 4);

static void SAIDMATXCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
static void SAIDMARXCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);

sai_transceiver_t SAI1_Tx_config = {
	.masterSlave = kSAI_Slave,
	.bitClock = {
		.bclkSrcSwap = false,
		.bclkSource = kSAI_BclkSourceBusclk,
		.bclkPolarity = kSAI_PolarityActiveLow,
		.bclkInputDelay = false
	},
	.frameSync = {
		.frameSyncWidth = 1U,
		.frameSyncPolarity = kSAI_PolarityActiveLow,
		.frameSyncEarly = true
	},
	.syncMode = kSAI_ModeAsync,
	.channelMask = kSAI_Channel0Mask,
	.startChannel = 0U,
	.endChannel = 0U,
	.channelNums = 1U,
	.serialData = {
		.dataMode = kSAI_DataPinStateTriState,
		.dataWord0Length = (uint8_t)kSAI_WordWidth16bits,
		.dataWordNLength = (uint8_t)kSAI_WordWidth16bits,
		.dataWordLength = (uint8_t)kSAI_WordWidth16bits,
		.dataOrder = kSAI_DataMSB,
		.dataFirstBitShifted = 16U,
		.dataWordNum = 2U,
		.dataMaskedWord = 0x0U
	},
	.fifo = {
		.fifoWatermark = 16U,
		.fifoCombine = kSAI_FifoCombineDisabled,
		.fifoPacking = kSAI_FifoPackingDisabled,
		.fifoContinueOneError = false
	}
};

/* SAI1 Rx configuration */
sai_transceiver_t SAI1_Rx_config = {
	.masterSlave = kSAI_Slave,
	.bitClock = {
		.bclkSrcSwap = false,
		.bclkSource = kSAI_BclkSourceBusclk,
		.bclkPolarity = kSAI_PolarityActiveLow,
		.bclkInputDelay = false
	},
	.frameSync = {
		.frameSyncWidth = 1U,
		.frameSyncPolarity = kSAI_PolarityActiveLow,
		.frameSyncEarly = true
	},
	.syncMode = kSAI_ModeSync,
	.channelMask = kSAI_Channel0Mask,
	.startChannel = 0U,
	.endChannel = 0U,
	.channelNums = 1U,
	.serialData = {
		.dataMode = kSAI_DataPinStateTriState,
		.dataWord0Length = (uint8_t)kSAI_WordWidth16bits,
		.dataWordNLength = (uint8_t)kSAI_WordWidth16bits,
		.dataWordLength = (uint8_t)kSAI_WordWidth16bits,
		.dataOrder = kSAI_DataMSB,
		.dataFirstBitShifted = 16U,
		.dataWordNum = 2U,
		.dataMaskedWord = 0x0U
	},
	.fifo = {
		.fifoWatermark = 16U,
		.fifoCombine = kSAI_FifoCombineDisabled,
		.fifoPacking = kSAI_FifoPackingDisabled,
		.fifoContinueOneError = false
	}
};

void SAI1Init() {
	//Init DMA Mux
	DMAMUX_Init(DMAMUX);
	DMAMUX_SetSource(DMAMUX, 0, (uint8_t)kDmaRequestMuxSai1Tx);
	DMAMUX_EnableChannel(DMAMUX, 0);
	DMAMUX_SetSource(DMAMUX, 1, (uint8_t)kDmaRequestMuxSai1Rx);
	DMAMUX_EnableChannel(DMAMUX, 1);

	//Init DMA/EDMA
	edma_config_t dmaConfig;
	EDMA_GetDefaultConfig(&dmaConfig);
	EDMA_Init(DMA0, &dmaConfig);
	EDMA_CreateHandle(&dmaTXHandle, DMA0, 0);
	EDMA_CreateHandle(&dmaRXHandle, DMA0, 1);

	//Init SAI clock gate
	SAI_Init(SAI1);
	//Init SAI DMA
	SAI_TransferTxCreateHandleEDMA(SAI1, &txHandle, SAIDMATXCallback, NULL, &dmaTXHandle);
	SAI_TransferRxCreateHandleEDMA(SAI1, &rxHandle, SAIDMARXCallback, NULL, &dmaRXHandle);
	//Configures SAI Tx sub-module functionality
//	SAI_TxSetConfig(SAI1, &SAI1_Tx_config);
	SAI_TransferTxSetConfigEDMA(SAI1, &txHandle, &SAI1_Tx_config);
	//Enable selected Tx interrupts
	SAI_TxEnableInterrupts(SAI1, (kSAI_FIFOErrorInterruptEnable));		//| kSAI_FIFORequestInterruptEnable
	//Configures SAI Rx sub-module functionality */
//	SAI_RxSetConfig(SAI1, &SAI1_Rx_config);
	SAI_TransferRxSetConfigEDMA(SAI1, &rxHandle, &SAI1_Rx_config);
	//Enable selected Rx interrupts
	SAI_RxEnableInterrupts(SAI1, (kSAI_FIFOErrorInterruptEnable));		//| kSAI_FIFORequestInterruptEnable
	//Enable interrupt SAI1_IRQn request in the NVIC.
	EnableIRQ(SAI1_IRQn);

	//Configure RX DMA
	sai_transfer_t xferRX = {.data = saiRXBuffer, .dataSize = (SAI_RX_BUFFER_SIZE * 2)};
	SAI_TransferReceiveEDMA(SAI1, &rxHandle, &xferRX);
	//Configure TX DMA
	sai_transfer_t xferTX = {.data = saiTXBuffer, .dataSize = (SAI_TX_BUFFER_SIZE * 2)};
	SAI_TransferSendEDMA(SAI1, &txHandle, &xferTX);
}

static struct rt_thread dspThread;
static rt_uint8_t dspThreadStack[DSP_THREAD_STACK_SIZE];

//const arm_cfft_instance_f32* cfftInstanceF32 = &arm_cfft_sR_f32_len64;
const arm_cfft_instance_q15* cfftInstanceQ15 = &arm_cfft_sR_q15_len256;

int16_t iqValuesQ15[SAI_RX_BUFFER_SIZE];
int16_t fftValuesQ15[DSP_FFT_LENGTH];
//float iqValuesF32[DSP_FFT_LENGTH];
void DSPThread() {
	InterThreadMessageStruct guiMsg;

	//Enable SAI1
	SAI1Init();

	while(1) {
		if(bufferFull == true) {
			memcpy(iqValuesQ15, saiRXBuffer, DSP_SIGNAL_LENGTH * 2);
			bufferFull = false;

			//Reset SAI
			SAI_TxSoftwareReset(SAI1, kSAI_ResetTypeSoftware);
			SAI_RxSoftwareReset(SAI1, kSAI_ResetTypeSoftware);
			//Configure RX DMA
			sai_transfer_t xferRX = {.data = saiRXBuffer, .dataSize = (SAI_RX_BUFFER_SIZE * 2)};
			SAI_TransferReceiveEDMA(SAI1, &rxHandle, &xferRX);
			//Configure TX DMA
			sai_transfer_t xferTX = {.data = saiTXBuffer, .dataSize = (SAI_TX_BUFFER_SIZE * 2)};
			SAI_TransferSendEDMA(SAI1, &txHandle, &xferTX);

			//Send I/Q signal values to GUI
			guiMsg.id = InterThread_TrackingSignal;
			guiMsg.data = (uint32_t*)iqValuesQ15;
			guiMsg.length = DSP_SIGNAL_LENGTH;
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));

			//Calculate Spectrum using Q15 (fixed-point)
			rt_tick_t startTick = rt_tick_get();
			memcpy(fftValuesQ15, iqValuesQ15, DSP_FFT_LENGTH);

			//Calculate complex FFT of IQ values
			arm_cfft_q15(cfftInstanceQ15, fftValuesQ15, 0, 1);
			arm_abs_q15(fftValuesQ15, fftValuesQ15, DSP_FFT_LENGTH);

			//Re-organize FFT output from [DC -> N/2 -> -DC] to [-N/2 -> DC -> N/2]
			uint8_t fftGain = 7;
			uint16_t midIndex = ((DSP_FFT_LENGTH >> 1) - 1);
			float mid = fftValuesQ15[midIndex] >> fftGain;
			uint8_t i;
			for(i = 0; i < midIndex; i++) {
				float tmp = fftValuesQ15[i] >> fftGain;
				fftValuesQ15[i] = fftValuesQ15[midIndex + 2 + i] >> fftGain;
				fftValuesQ15[midIndex + i] = tmp;
			}
			fftValuesQ15[DSP_FFT_LENGTH - 2] = mid;

	//		//Calculate Spectrum using float
	//		//Convert from Q15 to float
	//		arm_q15_to_float(iqValuesQ15, iqValuesF32, DSP_FFT_LENGTH);
	//
	//		//Calculate complex FFT of IQ values
	//		arm_cfft_f32(cfftInstanceF32, iqValuesF32, 0, 1);
	//		arm_abs_f32(iqValuesF32, iqValuesF32, DSP_FFT_LENGTH);
	//		uint8_t i;
	////		for(i = 0; i < DSP_FFT_LENGTH; i++) {
	////			iqValuesF32[i] = 20.0f * log10f(iqValuesF32[i] + 0.00001f);		//Add some value to avoid log(0)
	////		}
	//
	//		//Re-organize FFT output from [DC -> N/2 -> -DC] to [-N/2 -> DC -> N/2]
	//		uint16_t midIndex = ((DSP_FFT_LENGTH >> 1) - 1);
	//		float mid = iqValuesF32[midIndex];
	//		for(i = 0; i < midIndex; i++) {
	//			float tmp = iqValuesF32[i];
	//			iqValuesF32[i] = iqValuesF32[midIndex + 2 + i];
	//			iqValuesF32[midIndex + i] = tmp;
	//		}
	//		iqValuesF32[DSP_FFT_LENGTH - 2] = mid;
	//
	//		//Convert from float to int16
	//		for(i = 0; i < DSP_FFT_LENGTH; i++) {
	//			fftValuesQ15[i] = (int16_t)iqValuesF32[i];
	//		}

			volatile rt_tick_t deltaTick = rt_tick_get() - startTick;

			//Send calculated FFT to GUI
			guiMsg.id = InterThread_TrackingSpectrum;
			guiMsg.data = (uint32_t*)fftValuesQ15;
			guiMsg.length = DSP_FFT_LENGTH - 1;
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}

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

#define LED0_PIN               GET_PIN(1, 8)
rt_base_t pinValue = PIN_HIGH;
void SAI1_IRQHandler(void) {
//    if (SAI_TxGetStatusFlag(SAI1) & kSAI_FIFORequestFlag) {
//    	SAI_WriteData(SAI1, 0, 0xAAAA);
//    	SAI_WriteData(SAI1, 0, 0xAAAA);
//
////    	SAI_TxClearStatusFlags(SAI1, kSAI_FIFORequestFlag);
//    }

    if(SAI_TxGetStatusFlag(SAI1) & kSAI_FIFOErrorFlag) {
    	SAI_TxClearStatusFlags(SAI1, kSAI_FIFOErrorFlag);
		SAI_TxSoftwareReset(SAI1, kSAI_ResetTypeFIFO);
    }

//    if (SAI_RxGetStatusFlag(SAI1) & kSAI_FIFORequestFlag) {
//    	uint32_t word0 = SAI_ReadData(SAI1, 0);
//    	uint32_t word1 = SAI_ReadData(SAI1, 0);
//
//    	if(bufferFull == false) {
//			saiBuffer[saiBufferIndex + 0] = word0;
//			saiBuffer[saiBufferIndex + 1] = word1;
//
//			saiBufferIndex += 2;
//			if(saiBufferIndex >= SAI_RX_BUFFER_SIZE) {
//				saiBufferIndex = 0;
//				bufferFull = true;
//			}
//    	}
//
//    	if(pinValue == PIN_HIGH) {
//    		rt_pin_write(LED0_PIN, PIN_LOW);
//    		pinValue = PIN_LOW;
//    	}
//    	else {
//    		rt_pin_write(LED0_PIN, PIN_HIGH);
//    		pinValue = PIN_HIGH;
//    	}
//
//
////    	uint32_t channel1 = SAI_ReadData(SAI1, 1);
//
////		SAI_RxClearStatusFlags(SAI1, kSAI_FIFORequestFlag);
//    }

    if(SAI_RxGetStatusFlag(SAI1) & kSAI_FIFOErrorFlag) {
    	SAI_RxClearStatusFlags(SAI1, kSAI_FIFOErrorFlag);
		SAI_RxSoftwareReset(SAI1, kSAI_ResetTypeFIFO);
    }

    SDK_ISR_EXIT_BARRIER;
}

static void SAIDMATXCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData) {
//    sendCount++;
//    emptyBlock++;
//
//    if (sendCount == beginCount) {
//        istxFinished = true;
//        SAI_TransferTerminateSendEDMA(base, handle);
//        sendCount = 0;
//    }

	SAI_TransferTerminateSendEDMA(base, handle);
}

static void SAIDMARXCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData) {
//    receiveCount++;
//    fullBlock++;
//
//    if (receiveCount == beginCount) {
//        isrxFinished = true;
//        SAI_TransferTerminateReceiveEDMA(base, handle);
//        receiveCount = 0;
//    }

	if(pinValue == PIN_HIGH) {
		rt_pin_write(LED0_PIN, PIN_LOW);
		pinValue = PIN_LOW;
	}
	else {
		rt_pin_write(LED0_PIN, PIN_HIGH);
		pinValue = PIN_HIGH;
	}

	SAI_TransferTerminateReceiveEDMA(base, handle);
	bufferFull = true;
}
