#include "CATInterface.h"

CATInterfaceStatus CATASCIIToNumber(uint8_t* ascii, uint8_t asciiLength, int32_t* value);

//CAT Command Handlers
CATInterfaceStatus CATCommandAFCControl(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandCRC(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandEncoding(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandDatarateRX(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandDatarateTX(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandCenterFequencyA(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandCenterFequencyB(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandFraming(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandFunctionRX(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandFunctionTX(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandFirmware(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandAGCSpeed(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandIdentification(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandIFFrequency(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandRecallMemory(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandModulation(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandTXPower(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandBandwidth(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandReadMeter(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandRSSI(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandTNC(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandDeviation(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);
CATInterfaceStatus CATCommandPacketMeter(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);

/**
  * @brief	This function handles all incoming data/commands
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * CAT Command Example: Set: FA145895000; Read: FA; Return: FA145895000;
  */
CATInterfaceStatus CATInterfaceHandler(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	if(data[0] == 'A' && data[1] == 'F') {
		//AFC Control
		return CATCommandAFCControl(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'C' && data[1] == 'T') {
		//CRC/CCITT Control
		return CATCommandCRC(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'D' && data[1] == 'A') {
		//Screen Brightness setting
	}
	else if(data[0] == 'E' && data[1] == 'M') {
		//Bit Encoding Mode
		return CATCommandEncoding(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'E' && data[1] == 'X') {
		//Extended Menu
	}
	else if(data[0] == 'D' && data[1] == 'R') {
		//Datarate RX
		return CATCommandDatarateRX(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'D' && data[1] == 'T') {
		//Datarate TX
		return CATCommandDatarateTX(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'F' && data[1] == 'A') {
		//Center Frequency of Radio A
		return CATCommandCenterFequencyA(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'F' && data[1] == 'B') {
		//Center Frequency of Radio B
		return CATCommandCenterFequencyB(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'F' && data[1] == 'M') {
		//Framing Mode
		return CATCommandFraming(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'F' && data[1] == 'R') {
		//Function RX (ON or OFF)
		return CATCommandFunctionRX(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'F' && data[1] == 'T') {
		//Function TX (ON or OFF)
		return CATCommandFunctionTX(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'F' && data[1] == 'W') {
		//Radio firmware Identification (Firmware Version)
		return CATCommandFirmware(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'G' && data[1] == 'T') {
		//AGC Speed setting
		return CATCommandAGCSpeed(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'I' && data[1] == 'D') {
		//Radio module Identification (Hardware Version)
		return CATCommandIdentification(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'I' && data[1] == 'S') {
		//IF Frequency setting
		return CATCommandIFFrequency(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'K' && data[1] == 'S') {
		//CW/Morse Key Speed
	}
	else if(data[0] == 'M' && data[1] == 'D') {
		//Operating Mode/Modulation
		return CATCommandModulation(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'P' && data[1] == 'C') {
		//TX Output Power
		return CATCommandTXPower(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'P' && data[1] == 'M') {
		//Packet Meter/Tracking Value
		return CATCommandPacketMeter(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'R' && data[1] == 'G') {
		//RF Gain, LNA/AGC Gain
	}
	else if(data[0] == 'R' && data[1] == 'M') {
		//Read Meter/Tracking Value
		return CATCommandReadMeter(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'S' && data[1] == 'H') {
		//RX Bandwidth
		return CATCommandBandwidth(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'S' && data[1] == 'M') {
		//RSSI Reading
		return CATCommandRSSI(data, dataLength, rData, rDataLength);
	}
	else if(data[0] == 'T' && data[1] == 'C') {
		//TNC Mode
	}
	else if(data[0] == '?' && data[1] == ';') {
		//Command return ERROR
		*rDataLength = 0;
		return CAT_NACK;
	}
	else if(data[0] == 'O' && data[1] == 'K' && data[2] == ';') {
		//Command return OK
		*rDataLength = 0;
		return CAT_ACK;
	}
	else {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the AFC Range CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: AFC1125000; Read: AFC1; Return: AFC1125000;
  */
CATInterfaceStatus CATCommandAFCControl(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[9] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 6, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.afcRange = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_AFC, .data = (uint32_t*)radioAConfig.afcRange, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.afcRange = value;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the CRC Mode Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: CT10; Read: CT1; Return: CT10;
  */
CATInterfaceStatus CATCommandCRC(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[4] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 1, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.crc = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_CRC, .data = (uint32_t*)radioAConfig.crc, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.crc = value;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Encoding Mode CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: EM14; Read: EM1; Return: EM14;
  */
CATInterfaceStatus CATCommandEncoding(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[4] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 1, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.encoder = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_Encoding, .data = (uint32_t*)radioAConfig.encoder, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.encoder = value;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Datarate RX CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: DR1001200; Read: DR1; Return: DR1001200;
  */
CATInterfaceStatus CATCommandDatarateRX(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[9] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 6, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.rxDatarate = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_DatarateRX, .data = (uint32_t*)radioAConfig.rxDatarate, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.rxDatarate = value;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Datarate TX CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: DT0250000; Read: DT0; Return: DT0250000;
  */
CATInterfaceStatus CATCommandDatarateTX(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[9] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 6, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.txDatarate = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_DatarateTX, .data = (uint32_t*)radioAConfig.txDatarate, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.txDatarate = value;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Center Frequency A CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: FA145895000; Read: FA; Return: FA145895000;
  */
CATInterfaceStatus CATCommandCenterFequencyA(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	//Check if is Return Value, same syntax as Write command
	if(data[11] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[2], 9, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		radioAConfig.centerFrequency = value;

		//Inform GUI of value change
		InterThreadMessageStruct guiMsg = {.id = InterThread_CenterFrequency, .data = (uint32_t*)radioAConfig.centerFrequency, .length = 0 };
		rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Center Frequency B CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: FB145895000; Read: FB; Return: FB145895000;
  */
CATInterfaceStatus CATCommandCenterFequencyB(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	//Check if is Return Value, same syntax as Write command
	if(data[11] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[2], 9, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		radioBConfig.centerFrequency = value;

		//Inform GUI of value change
//		InterThreadMessageStruct guiMsg = {.id = InterThread_CenterFrequency, .data = (uint32_t*)radioAConfig.centerFrequency, .length = 0 };
//		rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Framing Mode CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: FM02; Read: FM0; Return: FM02;
  */
CATInterfaceStatus CATCommandFraming(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[4] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 1, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.framing = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_Framing, .data = (uint32_t*)radioAConfig.framing, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.framing = value;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Function RX (RX On/Off) CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: FR00; Read: FR0; Return: FR00;
  */
CATInterfaceStatus CATCommandFunctionRX(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[4] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 1, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			if(value == 0x01) {
				//Check if is AFSK mode, if yes change AFSK register because have different meaning in RX or TX
				if(radioAConfig.modulation == RadioModulation_AFSK) {

				}
			}
			radioAConfig.operationMode = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_Mode, .data = (uint32_t*)radioAConfig.operationMode, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			if(value == 0x01) {
				//Check if is AFSK mode, if yes change AFSK register because have different meaning in RX or TX
				if(radioBConfig.modulation == RadioModulation_AFSK) {

				}
			}
			radioBConfig.operationMode = value;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Function TX (TX On/Off) CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: FT01; Read: FT0; Return: FT01;
  */
CATInterfaceStatus CATCommandFunctionTX(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[4] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 1, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			if(value == 0x00) {
				radioAConfig.operationMode = RadioMode_OFF;
			}
			else if(value == 0x01) {
				radioAConfig.operationMode = RadioMode_TX;

				//Check if is AFSK mode, if yes change AFSK register because have different meaning in RX or TX
				if(radioAConfig.modulation == RadioModulation_AFSK) {

				}
			}
			else {
				return CAT_ERROR;
			}

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_Mode, .data = (uint32_t*)radioAConfig.operationMode, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			if(value == 0x00) {
				radioBConfig.operationMode = RadioMode_OFF;
			}
			else if(value == 0x01) {
				radioBConfig.operationMode = RadioMode_TX;

				//Check if is AFSK mode, if yes change AFSK register because have different meaning in RX or TX
				if(radioBConfig.modulation == RadioModulation_AFSK) {

				}
			}
			else {
				return CAT_ERROR;
			}
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Identification Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	0-> No Errors, 1->Error in Command
  *
  * Example: Set: NA; Read: FW; Return: FW010323232323230002323232323;
  */
uint8_t CATCommandFirmware(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	//Check if is Return Value, same syntax as Write command
	if(data[30] == ';') {
		//Return Value from Read Command
		uint32_t swVMajor;
		if(CATASCIIToNumber(&data[2], 2, &swVMajor) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		uint32_t swVMinor;
		if(CATASCIIToNumber(&data[4], 2, &swVMinor) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		uint32_t swVHash;
		if(CATASCIIToNumber(&data[6], 10, &swVHash) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		uint32_t catVMajor;
		if(CATASCIIToNumber(&data[16], 2, &catVMajor) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		uint32_t catVMinor;
		if(CATASCIIToNumber(&data[18], 2, &catVMinor) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		uint32_t catVHash;
		if(CATASCIIToNumber(&data[20], 10, &catVHash) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		firmwareVersionMajor = swVMajor;
		firmwareVersionMinor = swVMinor;
		firmwareVersionHash = swVHash;
		catInterfaceVersionMajor = catVMajor;
		catInterfaceVersionMinor = catVMinor;
		catInterfaceVersionHash = catVHash;
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the AGC Speed Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: GT001; Read: GT0; Return: GT001;
  */
CATInterfaceStatus CATCommandAGCSpeed(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[5] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 2, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.agcSpeed = (15 - value);	//Inverted Logic: CAT 0-> OFF, Local 0-> Fastest

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_AGCSpeed, .data = (uint32_t*)radioAConfig.agcSpeed, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.agcSpeed = (15 - value);	//Inverted Logic: CAT 0-> OFF, Local 0-> Fastest
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Identification Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	0-> No Errors, 1->Error in Command
  *
  * Example: Set: NA; Read: ID; Return: ID0103;
  */
uint8_t CATCommandIdentification(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	//Check if is Return Value, same syntax as Write command
	if(data[6] == ';') {
		//Return Value from Read Command
		uint32_t hwVMajor = 0;
		if(CATASCIIToNumber(&data[2], 2, &hwVMajor) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		uint32_t hwVMinor = 0;
		if(CATASCIIToNumber(&data[4], 2, &hwVMinor) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		hardwareVersionMajor = hwVMajor;
		hardwareVersionMinor = hwVMinor;
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the IF Frequency Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: IS1010000; Read: IS1; Return: IS1010000;
  */
CATInterfaceStatus CATCommandIFFrequency(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[9] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 6, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.ifFrequency = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_IFFrequency, .data = (uint32_t*)radioAConfig.ifFrequency, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.ifFrequency = value;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Modulation Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: MD01; Read: MD0; Return: MD01;
  */
CATInterfaceStatus CATCommandModulation(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[4] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 1, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.modulation = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_Modulation, .data = (uint32_t*)radioAConfig.modulation, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.modulation = value;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the TX Power Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: PC110; Read: PC1; Return: PC110;
  */
CATInterfaceStatus CATCommandTXPower(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[5] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 2, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.outputPower = (value - 10);

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_TXPower, .data = (uint32_t*)radioAConfig.outputPower, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.outputPower = (value - 10);
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the RX Bandwidth Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: SH0250000; Read: SH0; Return: SH0250000;
  */
CATInterfaceStatus CATCommandBandwidth(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[9] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 6, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			radioAConfig.bandwidth = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_Bandwidth, .data = (uint32_t*)radioAConfig.bandwidth, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBConfig.bandwidth = value;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Read Meter Value
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: NA; Read: RM10; Return: RM10100;
  */
CATInterfaceStatus CATCommandReadMeter(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	uint32_t meter = 0;
	if(CATASCIIToNumber(&data[3], 1, &meter) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	switch(meter) {
		case 0x00:
			//Meter RSSI Value
			if(data[7] == ';') {
				//Return Value from Read Command
				int32_t value = 0;
				if(CATASCIIToNumber(&data[4], 3, &value) != 0x00) {
					*rDataLength = sprintf(rData, "?;");
					return CAT_ERROR;
				}

				//RSSI value is always negative but sent as positive
				value = -value;

				if(radio == RADIO_A) {
					radioATracking.rssiTracking = value;

					//Inform GUI of value change
					InterThreadMessageStruct guiMsg = {.id = InterThread_TrackingRSSI, .data = (uint32_t*)radioATracking.rssiTracking, .length = 0 };
					rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
				}
				else if(radio == RADIO_B) {
					radioBTracking.rssiTracking = value;
				}
				else {
					*rDataLength = sprintf(rData, "?;");
					return CAT_ERROR;
				}
			}
			break;
		case 0x01:
			//Read RF Frequency Value
			if(data[11] == ';') {
				//Return Value from Read Command
				int32_t value = 0;
				if(CATASCIIToNumber(&data[5], 6, &value) != 0x00) {
					*rDataLength = sprintf(rData, "?;");
					return CAT_ERROR;
				}

				//Get value sign
				if(data[4] == '+') {
				}
				else if(data[4] == '-') {
					value = -value;
				}
				else {
					*rDataLength = sprintf(rData, "?;");
					return CAT_ERROR;
				}

				if(radio == RADIO_A) {
					radioATracking.rfFrequencyTracking = value;

					//Inform GUI of value change
					InterThreadMessageStruct guiMsg = {.id = InterThread_TrackingRFOffset, .data = (uint32_t*)radioATracking.rfFrequencyTracking, .length = 0 };
					rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
				}
				else if(radio == RADIO_B) {
					radioBTracking.rfFrequencyTracking = value;
				}
				else {
					*rDataLength = sprintf(rData, "?;");
					return CAT_ERROR;
				}
			}
			break;
		default:
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the RSSI CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: NA; Read: SM1; Return: SM1100;
  */
CATInterfaceStatus CATCommandRSSI(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[6] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 3, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		//RSSI value is always negative but sent as positive
		value = -value;

		if(radio == RADIO_A) {
			radioATracking.rssiTracking = value;

			//Inform GUI of value change
			InterThreadMessageStruct guiMsg = {.id = InterThread_TrackingRSSI, .data = (uint32_t*)radioATracking.rssiTracking, .length = 0 };
			rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
		}
		else if(radio == RADIO_B) {
			radioBTracking.rssiTracking = value;
		}
		else {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Function TNC (TNC On/Off) CAT Command
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	0-> No Errors, 1->Error in Command
  *
  * Example: Set: TC01; Read: TC0; Return: TC01;
  */
uint8_t CATCommandTNC(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[4] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 1, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio == RADIO_A) {
			if(value == 0x00) {
				//TNC Mode Off
				radioAConfig.tncMode = RadioTNCMode_OFF;
			}
			else if(value == 0x01) {
				//TNC Mode On
				radioAConfig.tncMode = RadioTNCMode_KISS;
			}
			else {
				rDataLength = sprintf(rData, "?;");
				return CAT_ERROR;
			}
		}
		else if(radio == RADIO_B) {
			if(value == 0x00) {
				//TNC Mode Off
				radioBConfig.tncMode = RadioTNCMode_OFF;
			}
			else if(value == 0x01) {
				//TNC Mode On
				radioBConfig.tncMode = RadioTNCMode_KISS;
			}
			else {
				rDataLength = sprintf(rData, "?;");
				return CAT_ERROR;
			}
		}
		else {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

CATInterfaceStatus CATCommandDeviation(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	//Check if is Return Value, same syntax as Write command
	if(data[5] == ';') {
		//Return Value from Read Command
		uint32_t value = 0;
		if(CATASCIIToNumber(&data[3], 2, &value) != 0x00) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}

		if(radio > 1) {
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
		}
	}
	else {
		//Syntax Error
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function handles the Packet Meter Value
  * @param	data: Current Input data string
  * @param	dataLength: Length of the data string
  * @param	rData: Return data string, what to answer over the interface
  * @param	rDataLength: Length of the return data string
  * @return	CATInterfaceStatus
  *
  * Example: Set: NA; Read: PM10; Return: PM10100;
  */
CATInterfaceStatus CATCommandPacketMeter(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength) {
	uint32_t radio = 0;
	if(CATASCIIToNumber(&data[2], 1, &radio) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	uint32_t meter = 0;
	if(CATASCIIToNumber(&data[3], 1, &meter) != 0x00) {
		*rDataLength = sprintf(rData, "?;");
		return CAT_ERROR;
	}

	switch(meter) {
		case 0x00:
			//Meter RSSI Value
			if(data[7] == ';') {
				//Return Value from Read Command
				int32_t value = 0;
				if(CATASCIIToNumber(&data[4], 3, &value) != 0x00) {
					*rDataLength = sprintf(rData, "?;");
					return CAT_ERROR;
				}

				//RSSI value is always negative but sent as positive
				value = -value;

				if(radio == RADIO_A) {
					radioAPacketTracking.rssiTracking = value;
				}
				else if(radio == RADIO_B) {
					radioBPacketTracking.rssiTracking = value;
				}
				else {
					*rDataLength = sprintf(rData, "?;");
					return CAT_ERROR;
				}
			}
			break;
		case 0x01:
			//Read RF Frequency Value
			if(data[11] == ';') {
				//Return Value from Read Command
				int32_t value = 0;
				if(CATASCIIToNumber(&data[5], 6, &value) != 0x00) {
					*rDataLength = sprintf(rData, "?;");
					return CAT_ERROR;
				}

				//Get value sign
				if(data[4] == '+') {
				}
				else if(data[4] == '-') {
					value = -value;
				}
				else {
					*rDataLength = sprintf(rData, "?;");
					return CAT_ERROR;
				}

				if(radio == RADIO_A) {
					radioAPacketTracking.rfFrequencyTracking = value;
				}
				else if(radio == RADIO_B) {
					radioBPacketTracking.rfFrequencyTracking = value;
				}
				else {
					*rDataLength = sprintf(rData, "?;");
					return CAT_ERROR;
				}
			}
			break;
		default:
			*rDataLength = sprintf(rData, "?;");
			return CAT_ERROR;
	}

	return CAT_OK;
}

/**
  * @brief	This function calculates the Power of base^exp, both uint8_t
  * @param	base: Base value of the Power
  * @param	exp: Exponent value of the Power
  * @return	The Power of base^exp
  */
uint32_t UIntPow(uint8_t base, uint8_t exp) {
	if(exp == 0) {
		return 1;
	}

	uint32_t value = base;
	uint8_t i;
	for(i = 1; i < exp; i++) {
		value *= base;
	}

	return value;
}

/**
  * @brief	This function converts the CAT value fields to int
  * @param	ascii: The CAT value field in ASCII
  * @param	asciiLength: Length of the ASCII field aka N digits
  * @param	value: Pointer to return value
  * @return	CATInterfaceStatus
  */
CATInterfaceStatus CATASCIIToNumber(uint8_t* ascii, uint8_t asciiLength, int32_t* value) {
	*value = 0;

	uint8_t i;
	for(i = 0; i < asciiLength; i++) {
		if(ascii[i] < '0' || ascii[i] > '9') {
			return CAT_ERROR;
		}
		uint32_t mult = UIntPow(10, (asciiLength - i - 1));
		*value += (ascii[i] - '0') * mult;
	}

	return CAT_OK;
}
