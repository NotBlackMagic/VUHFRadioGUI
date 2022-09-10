#include "radioInterface.h"

#define SERIAL_RX_PACKET_BUFFER_SIZE			200

#define SERIAL_TRACKING_REFRESH_PERIOD			500

//Serial Interface to VUHFRadio message status enumeration
typedef enum {
	SerialMessage_Free = 0x00,
	SerialMessage_WaitingACK = 0x01,
	SerialMessage_ACKed = 0x02,
	SerialMessage_Timeout = 0x03,
	SerialMessage_NACKed = 0xFF
} SerialMessageStatus;

//Local Functions
void RadioInterfaceInitInformation();
void RadioInterfaceInitConfigs();

//Thread related variables
static struct rt_thread radioInterfaceThread;
static rt_uint8_t radioInterfaceThreadStack[RADIO_INTERFACE_THREAD_STACK_SIZE];

//Serial Interface to VUHFRadio variables
static rt_device_t radioSerial;
//Serial TX variables
static struct rt_mutex serialMessageMutex;		//Mutex to protect shared variables bellow
volatile uint16_t txPacketLength;
volatile char txPacketBuffer[100];
volatile SerialMessageStatus radioSerialMessageStatus;
//Serial RX variables
uint8_t rxBuffer[10];
uint16_t rxPacketIndex;
uint8_t rxPacketBuffer[SERIAL_RX_PACKET_BUFFER_SIZE];
//For AX25 Messages
uint8_t ax25PacketPayload[256];
AX25Struct ax25Packet = {.payload = ax25PacketPayload};

//Thread input message queue variables
struct rt_messagequeue radioInterfaceMessageQueue;
static uint8_t radioInterfaceMessagePool[RADIO_INTERFACE_MESSAGE_POOL_SIZE];

//VUHFRadio tracking information request variables
uint8_t trackingDataID;
static struct rt_timer radioTrackingTimer;

/**
  * @brief	This function handles the serial RX callback, reads received bytes and assembles them into a packet
  * @param	dev: The device (serial) that called the callback function
  * @param	size: Number of bytes ready to be read
  * @return	rt_err_t
  */
static rt_err_t SerialRXCallback(rt_device_t dev, rt_size_t size) {
	//Read received bytes from device (serial) buffer into local rxBuffer
	uint8_t length = rt_device_read(dev, 0, &rxBuffer, 10);

	//Go through received bytes and organize them into a package
	uint8_t i;
	for(i = 0; i < length; i++) {
		//Search for start of packet
		if(rxPacketIndex == 0) {
			if(rxBuffer[i] >= 'A' && rxBuffer[i] <= 'Z') {
				//Start of CAT command packet (starts with capitalized letter)
				rxPacketBuffer[rxPacketIndex++] = rxBuffer[i];
			}
			else if(rxBuffer[i] == FEND) {
				//Start of a KISS packet
				rxPacketBuffer[rxPacketIndex++] = rxBuffer[i];
			}
			else {
				rxPacketIndex = 0;
			}
		}
		else {
			//Check for end of packet
			if(rxPacketBuffer[0] != FEND && rxBuffer[i] == ';') {
				//End of CAT command packet (ends with ';')
				rxPacketBuffer[rxPacketIndex++] = rxBuffer[i];

				//Add assembled packet to Radio Interface message queue
				InterThreadMessageStruct msg = {.id = InterThread_SerialCAT, .data = rxPacketBuffer, .length = rxPacketIndex};
				rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));

				rxPacketIndex = 0;

				return RT_EOK;
			}
			else if(rxBuffer[i] == FEND) {
				//End of a KISS packet
				rxPacketBuffer[rxPacketIndex++] = rxBuffer[i];

				//Add received message to Radio Interface message queue
				InterThreadMessageStruct msg = {.id = InterThread_SerialTNC, .data = rxPacketBuffer, .length = rxPacketIndex};
				rt_mq_send(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct));

				rxPacketIndex = 0;

				return RT_EOK;
			}
			else {
				//Middle of either packets
				rxPacketBuffer[rxPacketIndex++] = rxBuffer[i];

				//Check for buffer overlflow
				if(rxPacketIndex >= SERIAL_RX_PACKET_BUFFER_SIZE) {
					rxPacketIndex = 0;
				}
			}
		}
	}

	return RT_EOK;
}

/**
  * @brief	This function handles the radioTrackingTimer Timer timeout, sends a tracking information request to the VUHFRadio module
  * @param	parameter: Not used
  * @return	None
  */
static void RadioTrackingTimerTimout(void *parameter) {
	rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
	if(radioSerialMessageStatus == SerialMessage_Free) {
		//Request next tracking data
		txPacketLength = sprintf(txPacketBuffer, "RM0%d;", trackingDataID);
		rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
		radioSerialMessageStatus = SerialMessage_WaitingACK;

		//Step through available tracking data IDs
		trackingDataID += 1;
		if(trackingDataID > 2) {
			trackingDataID = 0;
		}
	}
	rt_mutex_release(&serialMessageMutex);
}

/**
  * @brief	The Radio Interface Thread function
  * @param	None
  * @return	None
  */
void RadioInterfaceThread() {
	rt_err_t status;
	InterThreadMessageStruct msg;

	uint8_t rData[20];
	uint16_t rDataLength;

	radioSerial = rt_device_find("uart3");
	status = rt_device_open(radioSerial, RT_DEVICE_FLAG_INT_RX);
	status = rt_device_set_rx_indicate(radioSerial, SerialRXCallback);

//	status = set_date(2022, 8, 5);
//	status = set_time(14, 10, 0);

	//Get Radio Information about hardware and firmware version
	RadioInterfaceInitInformation();

	//Get Radio initial configuration
	RadioInterfaceInitConfigs();

	//Start the tracking information update timer
	status = rt_timer_start(&radioTrackingTimer);

	while(1) {
		//Wait for new message in the thread message queue, or timeout to handle serial message ACK timeouts
	    status = rt_mq_recv(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct), 1000);

	    if(status != -RT_ETIMEOUT) {
	    	switch(msg.id) {
	    		//General OpCodes
				case InterThread_SerialCAT: {
					//Message from Serial Interface (Serial ISR)
					CATInterfaceStatus status = CATInterfaceHandler((uint8_t*)msg.data, msg.length, rData, &rDataLength);

					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(status == CAT_ACK) {
						//Received ACK
						radioSerialMessageStatus = SerialMessage_ACKed;
					}
					else if(status == CAT_NACK) {
						//Received NACK
						radioSerialMessageStatus = SerialMessage_NACKed;
					}
					else if(radioSerialMessageStatus == SerialMessage_WaitingACK && status == CAT_OK) {
						//Received Data, equivalent to a ACK
						radioSerialMessageStatus = SerialMessage_ACKed;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_SerialTNC: {
					//TNC message from Serial Interface (Serial ISR)
					uint8_t* pkt = (uint8_t*)msg.data;

					//Add dummy CRC, needed for AX25Decode and not provided over the KISS protocol
					pkt[msg.length++] = 0x00;
					pkt[msg.length++] = 0x00;

					AX25Decode(&pkt[2], (msg.length - 3), &ax25Packet);

					//Inform GUI of value change
					InterThreadMessageStruct guiMsg = {.id = InterThread_TNCPacket, .data = (uint32_t*)&ax25Packet, .length = sizeof(AX25Struct) };
					rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
					break;
				}
				//General Radio OpCodes
				case InterThread_Memory: {
					//Select memory (pre-defined settings) channel
					uint8_t memoryChannel = (uint8_t)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
//						txPacketLength = sprintf(txPacketBuffer, "FA%09d;", radioAConfig.centerFrequency);
//						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
//						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				//Analog Control OpCodes
				case InterThread_CenterFrequency: {
					//Center Frequency Change
					radioAConfig.centerFrequency = (uint32_t)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "FA%09d;", radioAConfig.centerFrequency);
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_AFC: {
					//AFC Change
					uint32_t value = (uint32_t)msg.data;
					switch(value) {
						case 0x00:
							//AFC Range Wide: 1/4 of bandwidth
							radioAConfig.afcRange = (radioAConfig.bandwidth >> 2);
							break;
						case 0x01:
							//AFC Range Middle: 1/8 of bandwidth
							radioAConfig.afcRange = (radioAConfig.bandwidth >> 3);
							break;
						case 0x02:
							//AFC Range Narrow: 1/16 of bandwidth
							radioAConfig.afcRange = (radioAConfig.bandwidth >> 4);
							break;
						case 0x03:
							//AFC Disabled
							radioAConfig.afcRange = 0;
							break;
						default:
							//AFC Disabled
							radioAConfig.afcRange = 0;
							break;
					}
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "AF0%06d;", radioAConfig.afcRange);
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_Mode: {
					//Mode (OFF/RX/TX) Change
					radioAConfig.operationMode = (RadioMode)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						if(radioAConfig.operationMode == RadioMode_OFF) {
							//Neither RX nor TX Mode
							txPacketLength = sprintf(txPacketBuffer, "FR00;");
//							txPacketLength = sprintf(txPacketBuffer, "FT00;");
						}
						else if(radioAConfig.operationMode == RadioMode_RX) {
							//Enable RX Mode
							txPacketLength = sprintf(txPacketBuffer, "FR01;");
						}
						else if(radioAConfig.operationMode == RadioMode_TX) {
							//Enable TX Mode
							txPacketLength = sprintf(txPacketBuffer, "FT01;");
						}
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_AGCSpeed: {
					//AGC Change
					uint8_t agcListToSpeed[6] = {0, 3, 5, 7, 9, 15};
					//Convert from AGC Drop Down index
					radioAConfig.agcSpeed = (uint8_t)agcListToSpeed[(uint8_t)msg.data];
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "GT0%02d;", (15 - radioAConfig.agcSpeed));
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_IFFrequency: {
					//IF Frequency Change
					radioAConfig.ifFrequency = (uint32_t)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "IS0%06d;", radioAConfig.ifFrequency);
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_Modulation: {
					//Modulation Change
					radioAConfig.modulation = (RadioModulation)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "MD0%01d;", radioAConfig.modulation);
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_TXPower: {
					//TX Power Change
					radioAConfig.outputPower = (int8_t)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "PC0%02d;", (radioAConfig.outputPower + 10));
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_Bandwidth: {
					//Bandwidth Change
					radioAConfig.bandwidth = (uint32_t)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "SH0%06d;", radioAConfig.bandwidth);
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_Deviation: {
					//FSK/FM Deviation Change
					break;
				}
				//Digital Radio Control OpCodes
				case InterThread_Framing: {
					//Packet Framing Change
					radioAConfig.framing = (RadioFraming)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "FM0%01d;", radioAConfig.framing);
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_CRC: {
					//Packet CRC Change
					radioAConfig.crc = (RadioCRC)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "CT0%01d;", radioAConfig.crc);
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_Encoding: {
					//Packet Encoding Change
					radioAConfig.encoder = (RadioEncoder)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "EM0%01d;", radioAConfig.encoder);
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_DatarateRX: {
					//Packet RX Datarate Change
					radioAConfig.rxDatarate = (uint32_t)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "DR0%06d;", radioAConfig.rxDatarate);
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_DatarateTX: {
					//Packet TX Datarate Change
					radioAConfig.txDatarate = (uint32_t)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						txPacketLength = sprintf(txPacketBuffer, "DT0%06d;", radioAConfig.txDatarate);
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
				case InterThread_TNCEnable: {
					//Enable TNC Mode
					radioAConfig.tncMode = (RadioTNCMode)msg.data;
					rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
					if(radioSerialMessageStatus == SerialMessage_Free) {
						if(radioAConfig.tncMode == RadioTNCMode_OFF) {
							//TNC Mode Off
							txPacketLength = sprintf(txPacketBuffer, "TC00;");
						}
						else if(radioAConfig.tncMode == RadioTNCMode_KISS) {
							//TNC Mode On
							txPacketLength = sprintf(txPacketBuffer, "TC01;");
						}
						rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
						radioSerialMessageStatus = SerialMessage_WaitingACK;
					}
					rt_mutex_release(&serialMessageMutex);
					break;
				}
	    	}
	    }
	    else {
			rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
			if(radioSerialMessageStatus == SerialMessage_WaitingACK) {
				radioSerialMessageStatus = SerialMessage_Timeout;
			}
			rt_mutex_release(&serialMessageMutex);
		}

	    rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
	    if(radioSerialMessageStatus != SerialMessage_WaitingACK) {
	    	if(txPacketBuffer[0] == 'F' && txPacketBuffer[1] == 'A') {
				//Last send command was change center frequency, check serial CAT message status to evaluate the VUHFRadio LOCK status
				uint8_t cfLock;
				if(radioSerialMessageStatus == SerialMessage_ACKed) {
					//Received ACK/OK so lock was achieved
					cfLock = 0x01;
				}
				else {
					//Some error so no lock was achieved
					cfLock = 0x00;
				}
				//Inform GUI of lock status
				InterThreadMessageStruct guiMsg = {.id = InterThread_CFLock, .data = (uint32_t*)cfLock, .length = 0 };
				rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
	    	}

	    	if(radioSerialMessageStatus == SerialMessage_ACKed || radioSerialMessageStatus == SerialMessage_NACKed || radioSerialMessageStatus == SerialMessage_Timeout) {
				radioSerialMessageStatus = SerialMessage_Free;
			}
	    }
	    rt_mutex_release(&serialMessageMutex);
	}
}

/**
  * @brief	The Radio Interface Thread initialization function
  * @param	None
  * @return	error
  */
static int RadioInterfaceThreadInit() {
	rt_err_t status;

    //Initialize the Radio Interface message queue
	status = rt_mq_init(   &radioInterfaceMessageQueue,
							"RdIntQu",
							&radioInterfaceMessagePool[0],
							sizeof(InterThreadMessageStruct),
							sizeof(radioInterfaceMessagePool),
							RT_IPC_FLAG_FIFO);

	//Initialize the tracking information update timer
	rt_timer_init(	&radioTrackingTimer,
    				"RdIntTm",
					RadioTrackingTimerTimout,
					RT_NULL,
					SERIAL_TRACKING_REFRESH_PERIOD,
					RT_TIMER_CTRL_SET_PERIODIC);

	//Initialize the serial message variable protection mutex
	status = rt_mutex_init(	&serialMessageMutex,
							"RdMsgMx",
							RT_IPC_FLAG_FIFO);

	//Initialize the actual Radio Interface thread
    status = rt_thread_init(&radioInterfaceThread,
							"RdIntTh",
							RadioInterfaceThread,
							RT_NULL,
							&radioInterfaceThreadStack[0],
							sizeof(radioInterfaceThreadStack),
							RADIO_INTERFACE_THREAD_PRIORITY,
							10);

    //Startup the Radio Interface thread
    status = rt_thread_startup(&radioInterfaceThread);

	return 0;
}
INIT_ENV_EXPORT(RadioInterfaceThreadInit);		//Use "hidden" call of the thread initialization function

/**
  * @brief	This function performs the initialization of the VUHFRadio module information variables, steps through all available information and sends a request message for each
  * @param	None
  * @return	None
  */
void RadioInterfaceInitInformation() {
	rt_err_t status;
	InterThreadMessageStruct msg;

	uint8_t rData[20];
	uint16_t rDataLength;

	//Init Radio Information
	uint8_t info = 0x00;
	uint8_t run = 0x01;
	while(run == 0x01) {
		status = rt_mq_recv(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct), 1000);
		if(status != -RT_ETIMEOUT) {
			if(msg.id == InterThread_SerialCAT) {
				//CAT message from Serial Interface (Serial ISR)
				CATInterfaceStatus status = CATInterfaceHandler((uint8_t*)msg.data, msg.length, rData, &rDataLength);

				rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
				if(status == CAT_ACK) {
					//Received ACK
					radioSerialMessageStatus = SerialMessage_ACKed;
				}
				else if(status == CAT_NACK) {
					//Received NACK
					radioSerialMessageStatus = SerialMessage_NACKed;
				}
				else if(radioSerialMessageStatus == SerialMessage_WaitingACK && status == CAT_OK) {
					//Received Data, equivalent to a ACK
					radioSerialMessageStatus = SerialMessage_ACKed;
				}
				rt_mutex_release(&serialMessageMutex);
			}
			else if(msg.id == InterThread_SerialTNC) {
				//TNC message from Serial Interface (Serial ISR)
				AX25Struct ax25Packet;
				AX25Decode(msg.data, msg.length, &ax25Packet);
			}
		}
		else {
			rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
			if(radioSerialMessageStatus == SerialMessage_WaitingACK) {
				radioSerialMessageStatus = SerialMessage_Timeout;
			}
			rt_mutex_release(&serialMessageMutex);
		}

		rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
		if(radioSerialMessageStatus == SerialMessage_ACKed || radioSerialMessageStatus == SerialMessage_NACKed || radioSerialMessageStatus == SerialMessage_Timeout) {
			radioSerialMessageStatus = SerialMessage_Free;
		}

		if(radioSerialMessageStatus == SerialMessage_Free) {
			//Check if all configurations received
			if(info == 0x02) {
				run = 0x00;
			}

			//Got answer, request next
			switch(info) {
				case 0x00:
					//Get configuration: hardwareVersionMajor, hardwareVersionMinor
					txPacketLength = sprintf(txPacketBuffer, "ID;");
					break;
				case 0x01:
					//Get configuration: firmwareVersionMajor, firmwareVersionMinor, firmwareVersionHash, catInterfaceVersionMajor, catInterfaceVersionMinor, catInterfaceVersionHash
					txPacketLength = sprintf(txPacketBuffer, "FW;");
					break;
				default:
					txPacketLength = 0;
					break;
			}

			rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
			radioSerialMessageStatus = SerialMessage_WaitingACK;

			info += 1;
		}
		rt_mutex_release(&serialMessageMutex);
	}
}

/**
  * @brief	This function performs the initialization of the VUHFRadio module configuration variables, steps through all configurations and sends a request message for each
  * @param	None
  * @return	None
  */
void RadioInterfaceInitConfigs() {
	rt_err_t status;
	InterThreadMessageStruct msg;

	uint8_t rData[20];
	uint16_t rDataLength;

	//Init Radio Configs
	uint8_t config = 0x00;
	uint8_t run = 0x01;
	while(run == 0x01) {
		status = rt_mq_recv(&radioInterfaceMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct), 1000);
		if(status != -RT_ETIMEOUT) {
			if(msg.id == InterThread_SerialCAT) {
				//CAT message from Serial Interface (Serial ISR)
				CATInterfaceStatus status = CATInterfaceHandler((uint8_t*)msg.data, msg.length, rData, &rDataLength);

				rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
				if(status == CAT_ACK) {
					//Received ACK
					radioSerialMessageStatus = SerialMessage_ACKed;
				}
				else if(status == CAT_NACK) {
					//Received NACK
					radioSerialMessageStatus = SerialMessage_NACKed;
				}
				else if(radioSerialMessageStatus == SerialMessage_WaitingACK && status == CAT_OK) {
					//Received Data, equivalent to a ACK
					radioSerialMessageStatus = SerialMessage_ACKed;
				}
				rt_mutex_release(&serialMessageMutex);
			}
			else if(msg.id == InterThread_SerialTNC) {
				//TNC message from Serial Interface (Serial ISR)
				AX25Struct ax25Packet;
				AX25Decode(msg.data, msg.length, &ax25Packet);
			}
		}
		else {
			rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
			if(radioSerialMessageStatus == SerialMessage_WaitingACK) {
				radioSerialMessageStatus = SerialMessage_Timeout;
			}
			rt_mutex_release(&serialMessageMutex);
		}

		rt_mutex_take(&serialMessageMutex, RT_WAITING_FOREVER);
		if(radioSerialMessageStatus == SerialMessage_ACKed || radioSerialMessageStatus == SerialMessage_NACKed || radioSerialMessageStatus == SerialMessage_Timeout) {
			radioSerialMessageStatus = SerialMessage_Free;
		}

		if(radioSerialMessageStatus == SerialMessage_Free) {
			//Check if all configurations received
			if(config == 0x14) {
				run = 0x00;
			}

			//Got answer, request next
			switch(config) {
				case 0x00:
					//Get configuration: operationModeA
					txPacketLength = sprintf(txPacketBuffer, "FT0;");
					break;
				case 0x01:
					//Get configuration: operationModeA
					txPacketLength = sprintf(txPacketBuffer, "FR0;");
					break;
				case 0x02:
					//Get configuration: centerFrequencyA
					txPacketLength = sprintf(txPacketBuffer, "FA;");
					break;
				case 0x03:
					//Get configuration: modulationA
					txPacketLength = sprintf(txPacketBuffer, "MD0;");
					break;
				case 0x04:
					//Get configuration: ifFrequncyA
					txPacketLength = sprintf(txPacketBuffer, "IS0;");
					break;
				case 0x05:
					//Get configuration: bandwidthA
					txPacketLength = sprintf(txPacketBuffer, "SH0;");
					break;
				case 0x06:
					//Get configuration: rxDatarateA
					txPacketLength = sprintf(txPacketBuffer, "DR0;");
					break;
				case 0x07:
					//Get configuration: serialMesssagerateA
					txPacketLength = sprintf(txPacketBuffer, "DT0;");
					break;
				case 0x08:
					//Get configuration: afcRangeA
					txPacketLength = sprintf(txPacketBuffer, "AF0;");
					break;
				case 0x09:
					//Get configuration: outputPowerA
					txPacketLength = sprintf(txPacketBuffer, "PC0;");
					break;
				case 0x0A:
					//Get configuration: agcSpeedA
					txPacketLength = sprintf(txPacketBuffer, "GT0;");
					break;
				case 0x0B:
					//Get configuration: frequencyDeviationA
//					txPacketLength = sprintf(txPacketBuffer, "SH0;");
					radioAConfig.frequencyDeviation = 4800;
					txPacketLength = 0;
					break;
				case 0x0C:
					//Get configuration: afskSpaceA
//					txPacketLength = sprintf(txPacketBuffer, "SH0;");
					radioAConfig.afskSpace = 2200;
					txPacketLength = 0;
					break;
				case 0x0D:
					//Get configuration: afskMarkA
//					txPacketLength = sprintf(txPacketBuffer, "SH0;");
					radioAConfig.afskMark = 1200;
					txPacketLength = 0;
					break;
				case 0x0E:
					//Get configuration: afskDetectorBWA
//					txPacketLength = sprintf(txPacketBuffer, "SH0;");
					radioAConfig.afskDetectorBW = 4800;
					txPacketLength = 0;
					break;
				case 0x0F:
					//Get configuration: morseSpeedA
//					txPacketLength = sprintf(txPacketBuffer, "SH0;");
					radioAConfig.morseSpeed = 0;
					txPacketLength = 0;
					break;
				case 0x10:
					//Get configuration: encoderA
					txPacketLength = sprintf(txPacketBuffer, "EM0;");
					break;
				case 0x11:
					//Get configuration: framingA
					txPacketLength = sprintf(txPacketBuffer, "FM0;");
					break;
				case 0x12:
					//Get configuration: crcA
					txPacketLength = sprintf(txPacketBuffer, "CT0;");
					break;
				case 0x13:
					//Get configuration: tncMode
					txPacketLength = sprintf(txPacketBuffer, "TC0;");
					break;
				default:
					txPacketLength = 0;
					break;
			}

			if(txPacketLength > 0) {
				rt_err_t status = rt_device_write(radioSerial, 0, txPacketBuffer, txPacketLength);
				radioSerialMessageStatus = SerialMessage_WaitingACK;
			}

			config += 1;
		}
		rt_mutex_release(&serialMessageMutex);
	}
}
