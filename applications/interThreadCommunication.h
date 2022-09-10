#ifndef APPLICATIONS_INTERTHREADCOMMUNICATION_H_
#define APPLICATIONS_INTERTHREADCOMMUNICATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtdevice.h"

#define RADIO_INTERFACE_THREAD_PRIORITY			10
#define RADIO_INTERFACE_THREAD_STACK_SIZE		1024
#define RADIO_INTERFACE_MESSAGE_POOL_SIZE		1024

#define GUI_MESSAGE_POOL_SIZE					1024

#define DSP_THREAD_PRIORITY						10
#define DSP_THREAD_STACK_SIZE					1024

typedef struct {
    uint32_t id;
    uint32_t *data;
    uint32_t length;
} InterThreadMessageStruct;

extern struct rt_messagequeue guiMessageQueue;
extern struct rt_messagequeue radioInterfaceMessageQueue;

typedef enum {
	//General OpCodes
	InterThread_SerialCAT = 0x00,
	InterThread_SerialTNC = 0x01,
	InterThread_Time = 0x02,
	//General Radio OpCodes
	InterThread_Memory = 0x11,
	//Analog Control OpCodes
	InterThread_CenterFrequency = 0x21,
	InterThread_AFC = 0x22,
	InterThread_Mode = 0x23,
	InterThread_AGCSpeed = 0x24,
	InterThread_IFFrequency = 0x25,
	InterThread_Modulation = 0x26,
	InterThread_TXPower = 0x27,
	InterThread_Bandwidth = 0x28,
	InterThread_Deviation = 0x29,
	InterThread_CFLock = 0x2A,
	//Digital Radio Control OpCodes
	InterThread_Framing = 0x31,
	InterThread_CRC = 0x32,
	InterThread_Encoding = 0x33,
	InterThread_DatarateRX = 0x34,
	InterThread_DatarateTX = 0x35,
	//Tracking Information OpCodes
	InterThread_TrackingRSSI = 0x41,
	InterThread_TrackingRFOffset = 0x42,
	InterThread_TrackingAGCGain = 0x43,
	InterThread_TrackingSpectrum = 0x4A,
	InterThread_TrackingSignal = 0x4B,
	//TNC Control and Information
	InterThread_TNCEnable = 0x51,
	InterThread_TNCPacket = 0x52
} InterThreadOpcodes;

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_INTERTHREADCOMMUNICATION_H_ */
