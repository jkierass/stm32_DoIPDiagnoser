# STM32H747I-DISCO TBS

The default IDE is set to STM32CubeIDE, to change IDE open the STM32H747I-DISCO.ioc with STM32CubeMX and select from the supported IDEs (STM32CubeIDE, EWARM, and MDK-ARM). Supports flashing of the STM32H747I-DISCO board directly from TouchGFX Designer using GCC and STM32CubeProgrammer. Flashing the board requires STM32CubeProgrammer which can be downloaded from the ST webpage.

This TBS is configured for 480 x 800 pixels 24bpp screen resolution.

Performance testing can be done using the GPIO pins designated with the following signals: VSYNC_FREQ - Pin PJ3 (D2), RENDER_TIME - Pin PJ8 (D1), FRAME_RATE - Pin PJ9 (D0), MCU_ACTIVE - Pin PF8 (D3).

## Dual-Core Changes

This TouchGFX Board Setup (TBS) supports the dual-core functionality of the STM32H747I-DISCO.
For this reason, it is important to understand that there are two separate sub-projects for each compiler:
- A CM4 project
- A CM7 project

Since TouchGFX is running on the CM7, its code is located within the CM7 folder inside the project structure:

- 📁 CM4
    - 📁Core
- 📁 CM7
    - 📁 Core
    - 📁 TouchGFX

The CM4 and CM7 sub-project are split up as below:
- 📁 STM32CubeIDE
    - 📁 CM4
    - 📁 CM7

# Configuration

## Required hardware configuration changes

To make Ethernet work on STM32H747, the user needs to desolder SB21 and solder SB8 (all the information is available here: https://www.st.com/resource/en/user_manual/dm00504240-discovery-kit-with-stm32h747xi-mcu-stmicroelectronics.pdf).

To make temperature measurement work, MCP9808 has to be connected to the board through PD13 and PD14 pins (it is I2C4 configuration) and assigned to CM4 core.

## Required software configuration changes

This project uses C++17 features, so the whole project has to be configured to support this standard (also it can be newer).

 
It is also worth setting up debugging as shown in this document to debug two processors at the same time: https://www.st.com/resource/en/application_note/dm00629855-getting-started-with-projects-based-on-dualcore-stm32h7-microcontrollers-in-stm32cubeide-stmicroelectronics.pdf
That document explains the whole process in debugging section, the user needs to follow everything from the beginning of this section except the last step, which is "ST-LINK GDB server launch group". In this setup, debugging CM7 will also upload the software to CM4, and to debug CM4 as well, the user needs to start debugging on CM4 after the user has started debugging CM7. "Run" will upload the software to only one core, so it is better to use "Debug".

It is also necessary to add "-u _printf_float" flag to CM4 and CM7 linkers.

## Emulator
Requires:
- pandas
 
To simulate sending data from a car using a PC, follow these steps:

1. Change the Ethernet network card settings:

- IP Address: 169.254.19.42
- Subnet Mask: 255.255.0.0

It is recommended to use this specific address, but other addresses from the 169.254.0.0 subnet should also work. The only limitation is to avoid using 169.254.1.1, as this address is statically assigned to the STM32H747 board in this project.

2. Run the tcp_test_advanced.py script.

This script will emulate the car by sending diagnostic data. In the main() function, there is a path to a .csv file with example diagnostic parameters that will be sent. The user can freely modify the parameter values in this file.
To add a new diagnostic parameter, the user needs to:

- Add it to the appropriate structures in the script (check how other parameters are added and replicate the same process for the new one).
- Add new parameter's values the .csv file.

## Receiving data from serial port
Requires:
- pandas
- pyserial

To retrieve measurement data sent by the board over UART (technically, from the PC's perspective, this is a serial port since the ST-LINK debugger is used for this purpose), the user needs to run the read_data_from_serial.py script. If a different serial communication port is used in the user's setup instead of "COM3," the user needs to update the script accordingly.

Once the script is started, data collection will begin. When the user presses Ctrl + C, the data collection will stop, and the collected data will be saved in the corresponding .csv files.