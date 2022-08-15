# GUI Interface for the VUHFRadio: 2nd Generation

This is the firmware for the 2nd Generation GUI Interface for the [VUHFRadio](www.notblackmagic.com/projects/vuhf-radio/) Module using the [MIMXRT1060-EVKB](www.nxp.com/design/development-boards/i-mx-evaluation-and-development-boards/i-mx-rt1060-evaluation-kit:MIMXRT1060-EVKB) development board and touchscreen from NXP. This kit was kindly provided by [NXP](www.nxp.com) and [RT-Thread](www.rt-thread.io) as part of the [Embedded GUI Contest](https://rt-thread.medium.com/embedded-gui-contest-4c22ba04ca37).

The following features are tested and working:
- Loading of configurations and board information on startup from the VUHFRadio Module
- Update and display RSSI and RF Frequency tracking variables
- Working menus for changing main analog domain and digital domain configurations and send the changes to the VUHFRadio module
- Decode and display AX.25 frames received from the VUHFRadio Module over the KISS protocol

Features under development but not currently working/finished:
- Advanced settings menu for more analog and digital domain configurations for the VUHFRadio module
- A simple message form/window to input AX.25 messages and send them
- Spectrum of the receiver signal using the digital DSP interface of the AX5043 and the SAI peripheral of the RT1060 MCU

## Hardware

### BOM
- 1x MIMXRT1060 Development Board
- 1x RK043FN66HS-CTG Touch Screen Module
- 2x USB Micro-B cables
- 4x Dupont F-M jumper cables
- 1x VUHRadio Module V1.3

This second generation GUI Interface for the VUHFRadio module is being developed with the MIMXRT1060-EVKB development kit from NXP. The kit comes with the MIMXRT1060 development board and a 5" 480x272 capacitive touch screen module, RK043FN66HS-CTG. 

The VUHFRadio Module is another project I'm developing, it is a dual band (VHF and UHF) dual radio transceiver based around the [AX5043](www.onsemi.com/products/wireless-connectivity/wireless-rf-transceivers/ax5043) Sub-GHz transceiver IC. Both the hardware and firmware for the module is openly available from the [Website](www.notblackmagic.com/projects/vuhf-radio/) and [GitHub](https://github.com/NotBlackMagic/VUHFRadio) respectively. The VUHFRadio module is connected from its UART header to the MIMXRT1060 development board Arduino Header: The UART pins connected to the Arduino Header J16 Pin 1 (RX) & 2 (TX), and the 3V3 and GND also to the Arduino Header, J32 Pin 4 & 6 respectively.

A custom version of the VUHFRadio Module for the M.2 slot of the MIMXRT1060 board is planned, as well as an additional adapter from the standard arduino headers to a M.2 slot so that the VUHFRadio Module can be used with other development board with the arduino headers.

## Software
The GUI Interface runs on the RT-Thread RTOS and uses the [LVGL](www.lvgl.io) embedded graphics library. The base project, on which this project is built upon, with RT-Thread and LVGL running on the MIMXRT1060-EVKB board is available [here](www.github.com/NotBlackMagic/MIMXRT1060-EVKB). It was developed using the custom RT-Thread IDE, [RT-Thread Studio](www.rt-thread.io/studio.html), and the full project files and dependencies are included in this GitHub repository. To run the project it is therefore only necessary to install RT-Thread Studio, download this repository and import this project in RT-Thread Studio. To debug and flash the board from RT-Thread Studio follow the instruction bellow in the "Flashing and Debugging" Section.

### Architecture
The GUI Interface firmware is composed of three main modules, each responsible for a specific function and running as its own Thread:

- GUI: Responsible to draw teh GUI, react to GUI events and update the GUI
- Radio Interface: Responsible for interacting with the VUHFRadio module through a serial interface
- DSP: Responsible for signal processing of the I/Q signals received from the VUHFRadio module through either analog or digital interface (NOT IMPLEMENTED)

To manage communication between the threads, each thread has its own message queue where other threads, and the ISR in some cases, publish necessary messages to. A standard message type is defined, "InterThreadMessageStruct", which holds an ID/OpCode, a pointer to data and a data length field. In case the data fits directly into the data pointer it is passed by value, else by reference as a pointer.

The communication from the VUHFRadio module to the GUI Interface are handled as follows: 
- Bytes are read and processed by the serial ISR and a full packet is assembled
- The full packet is published by reference from the serial ISR to the Radio Interface message queue
- The Radio Interface Thread reads the message from the queue and decodes it
- If changes to the GUI are required, the Radio Interface Thread publishes the required changes to the GUI Interface message queue

The communication in the reverse direction, from GUI Interface to VUHFRadio module, are handled basically in reveres:
- If the GUI notices a change, an event, it publishes that change to the Radio Interface message queue
- The Radio Interface thread then reads that message and writes the appropriate command message over the serial interface to the VUHFRadio module

This means that any updates to the GUI are performed ONLY by the GUI Thread, keeping the GUI thread safe as the LVGL library by itself is not. The updates to the GUI are performed before the LVGL task runs, this is done by modifying the **lvgl_thread_entry** function (packages\LVGL-v8.3.0\env_support\rt-thread\lv_rt_thread_port.c) and adding a external gui update handling function in the while loop:

```c
/* handle the tasks of LVGL */
while(1)
{
	lv_user_gui_update();							//User GUI Update Call
	lv_task_handler();								//LVGL GUI Thread Handler
	rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);		//Refresh rate of the GUI
}
```

Another way to keep the GUI thread safe would be to wrap the **lv_task_handler** function and any GUI draw/update calls with a mutex, as described in the LVGL porting [documentation](https://docs.lvgl.io/master/porting/os.html).

To manage the GUI better it is split into different areas, with each area being separated into there own source and header files:

![GUI UI](./GUI_UI.png)

### Flashing and Debugging
The project uses the J-Link Debugger settings so it is necessary to flash the MIMXRT1060-EVKB on-board debug probe with the J-Link Firmware. 

The following article provides the instructions on how to flash the J-Link Firmware onto the on-board debug probe:
https://community.nxp.com/t5/i-MX-RT-Knowledge-Base/Using-J-Link-with-MIMXRT1060-EVKB/ta-p/1452717

### Recover Unresponsive/"Bricked" Board
When flashing an erroneous code, it can happen that the EVK board gets unresponsive, impossible to create a debug link and erase and flash a good code to it. To recover from such events, I found the following steps work:

- Re-flash the on-board debug probe with the CMSIS-DAP firmware
- Change the boot option of the EVK board to the SD card (SW4: 1010), aka to nothing, and power cycle the board
- Use the MCUXpresso "GUI Flash Tool" to fully erase the flash
- Change the boot option back to the QSPI Flash (SW4: 0010) and power cycle the board
- The board should now allow a debug connection again
