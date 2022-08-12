# GUI Interface for the VUHFRadio: 2nd Generation

This is the firmware for the 2nd Generation GUI Interface for the [VUHFRadio](www.notblackmagic.com/projects/vuhf-radio/) Module using the [MIMXRT1060-EVKB](www.nxp.com/design/development-boards/i-mx-evaluation-and-development-boards/i-mx-rt1060-evaluation-kit:MIMXRT1060-EVKB) development board and touchscreen from NXP. This kit was kindly provided by [NXP](www.nxp.com) and [RT-Thread](www.rt-thread.io) as part of the [Embedded GUI Contest](https://rt-thread.medium.com/embedded-gui-contest-4c22ba04ca37).

Current version is tested and works with:
- Loading of configurations and board information on startup from the VUHFRadio Module
- Update and display RSSI and RF Frequency tracking variables
- Working menus for changing main analog domain and digital domain configurations and send the changes to the VUHFRadio module
- Decode and display AX.25 frames received from the VUHFRadio Module over the KISS protocol

Features under development but not currently working/finished:
- Advanced settings menu for more analog and digital domain configurations for the VUHFRadio module
- A simple message form/window to input AX.25 messages and send them
- Spectrum of the receiver signal using the digital DSP interface of the AX5043 and the SAI peripheral of the RT1060 MCU

## Hardware
This second generation GUI Interface for the VUHFRadio module is beeing developed with the MIMXRT1060-EVKB development kit from NXP. The kit comes with the MIMXRT1060 development board and a 5" 480x272 capacitive touch screen module, RK043FN66HS-CTG. 

The VUHFRadio Module is another project I'm developing, it is a dual band (VHF and UHF) dual radio trasceiver based around the [AX5043](www.onsemi.com/products/wireless-connectivity/wireless-rf-transceivers/ax5043) Sub-GHz transceiver IC. Both the hardware and firmware for the module is openly available from the [Website](www.notblackmagic.com/projects/vuhf-radio/) and [GitHub](https://github.com/NotBlackMagic/VUHFRadio) respectivly.

A custom version of the VUHFRadio Module for the M.2 slot of the MIMXRT1060 board is planned, as well as an additional adapter from the standard arduino headers to a M.2 slot so that the VUHFRadio Module can be used with other development board with the arduino headers.

### BOM
- 1x MIMXRT1060 Development Board
- 1x GAGGBA Touch Screen Module
- 2x USB Micro-B cables
- 3x Dupont F-M jumper cables
- 1x VUHRadio Module V1.3

## Software
The GUI Interface runs on the RT-Thread RTOS and uses the [LVGL](www.lvgl.io) embedded graphics library. The base project, on which this project is built upon, with RT-Thread and LVGL running on the MIMXRT1060-EVKB board is available [here](www.github.com/NotBlackMagic/MIMXRT1060-EVKB). It was developed using the custom RT-Thread IDE, [RT-Thread Studio](www.rt-thread.io/studio.html), and the full project files and dependecies are inculded in this GitHub repository. To run the project it is therefore only necessary to install RT-Thread Studio, download this repository and import this project in RT-Thread Studio. To debug and flash the board from RT-Thread Studio follow the isntruction bellow in the "Flashing and Debugging" Section.

### Architecture

Some characteristics of this firmware:
- Custom styling for all GUI items (dark mode)
- Custom AX.25 decoding/encode library, from the VUHFRadio project
- The drivers used are from the RT-Thread RTOS (Serial/UART and RTC)
- Command interface to the VUHFRadio module uses the CAT command interface used in the VUHRadio Module over a private UART interface
- TNC style interface to the VUHFRadio uses the KISS protocol

### File Organization

### Flashing and Debugging
The project uses the J-Link Debugger settings so it is necessary to flash the MIMXRT1060-EVKB on-board debug probe with the J-Link Firmware. 

The following article provides the instructions on how to flash the J-Link Firmware onto the on-board debug probe:
https://community.nxp.com/t5/i-MX-RT-Knowledge-Base/Using-J-Link-with-MIMXRT1060-EVKB/ta-p/1452717

### Recover Unresponsive/"Bricked" Board
When flashing an erroneous code, it can happen that the EVK board gets unresponsive, impossible to create a debug link and erase and flash a good code to it. To recover from such events, I found the following steps work:

- Reflash the on-board debug probe with the CMSIS-DAP firmware
- Change the boot option of the EVK board to the SD card (SW4: 1010), aka to nothing, and power cycle the board
- Use the MCUXpresso "GUI Flash Tool" to fully erase the flash
- Change the boot option back to the QSPI Flash (SW4: 0010) and power cycle the board
- The board should now allow a debug connection again
