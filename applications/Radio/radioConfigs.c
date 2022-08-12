#include "radioConfigs.h"

//Radio Module Information
volatile uint8_t firmwareVersionMajor;
volatile uint8_t firmwareVersionMinor;
volatile uint32_t firmwareVersionHash;
volatile uint8_t catInterfaceVersionMajor;
volatile uint8_t catInterfaceVersionMinor;
volatile uint32_t catInterfaceVersionHash;
volatile uint8_t hardwareVersionMajor;
volatile uint8_t hardwareVersionMinor;

//Radio A (UHF) Configurations
volatile RadioBaseConfigs radioABaseConfigs;			//Base Radio Configs Module
volatile RadioConfigsStruct radioAConfig;

//Tracking Variables
volatile RadioTrackingStruct radioATracking;
volatile RadioTrackingStruct radioAPacketTracking;

//Radio B (UHF) Configurations
volatile RadioBaseConfigs radioBBaseConfigs;			//Base Radio Configs Module
volatile RadioConfigsStruct radioBConfig;

//Tracking Variables
volatile RadioTrackingStruct radioBTracking;
volatile RadioTrackingStruct radioBPacketTracking;
