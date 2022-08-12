#ifndef COMMANDS_CATCOMMANDINTERFACE_H_
#define COMMANDS_CATCOMMANDINTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#include "gui.h"
#include "radioConfigs.h"
#include "radioInterface.h"

typedef enum {
	CAT_OK = 0x00,
	CAT_ACK = 0x01,
	CAT_NACK = 0x02,
	CAT_ERROR = 0xFF
} CATInterfaceStatus;

uint8_t CATInterfaceHandler(uint8_t* data, uint16_t dataLength, uint8_t* rData, uint16_t* rDataLength);

#ifdef __cplusplus
}
#endif

#endif /* COMMANDS_CATCOMMANDINTERFACE_H_ */
