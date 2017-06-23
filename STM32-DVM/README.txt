Create a work directory in Windows C:\Work

Download MMDVM from <https://github.com/g4klx/MMDVM/archive/master.zip>
Extract to C:\Work
Rename MMDVM-master to MMDVM

Download <https://github.com/juribeparada/STM32F4XX_Lib/archive/master.zip>
Extract to C:\Work\MMDVM
Rename STM32F4XX_Lib-master to STM32F4XX_Lib

Download and install <https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/gcc-arm-none-eabi-5_4-2016q3-20160926-win32.exe>
MAKE SURE TO CHECK ALL BOXES AT THE END OF THE INSTALL

Download <https://github.com/gnu-mcu-eclipse/windows-build-tools/releases/download/v2.9/gnu-mcu-eclipse-build-tools-2.9-20170607-0922-win32.zip>
Extract to C:\Work
Copy the files in C:\Work\gnu-mcu-eclipse\build-tools\2.9-20170607-0922\bin to C:\Program Files (x86)\GNU Tools ARM Embedded\5.4 2016q3\bin

Download <https://sourceforge.net/projects/micro-os-plus/files/Vendor%20Archives/STM32/stm32cube_fw_f1_v140.zip/download>
Extract to C:\Work

Edit C:\Work\MMDVM\Config.h to look like this:

// For 12 MHz
#define EXTERNAL_OSC 12000000
// For 12.288 MHz
// #define EXTERNAL_OSC 12288000
// For 14.4 MHz
// #define EXTERNAL_OSC 14400000
// For 19.2 MHz
// #define EXTERNAL_OSC 19200000

// Allow the use of the COS line to lockout the modem
// #define USE_COS_AS_LOCKOUT

// Use pins to output the current mode
// #define ARDUINO_MODE_PINS

// For the original Arduino Due pin layout
// #define ARDUINO_DUE_PAPA

// For the ZUM V1.0 and V1.0.1 boards pin layout
// #define ARDUINO_DUE_ZUM_V10

// For the SQ6POG board
#define STM32F1_POG

Edit C:\Work\MMDVM\Makefile.CMSIS to look like this:

# Include directory for CMSIS
CMSISDIR:=C:\Work\STM32Cube_FW_F1_V1.4.0/Drivers/CMSIS

Open a CMD prompt
C:
cd \Work\MMDVM
make -v
Should look like this:

C:\Work\MMDVM>make -v
GNU Make 4.1
Built for x86_64-w64-mingw32
Copyright (C) 1988-2014 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

If all is well:
make -f Makefile.CMSIS

SHOULD compile

