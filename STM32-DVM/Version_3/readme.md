This is for the Version 3 (Blue Board) ONLY!
Clone the MMDVM repository from G4LKX

Add STM32F4XX_Lib directory

Copy Config.h IO.cpp IOSTM.cpp Makefile SerialSTM.cpp over the existing files.

make dvm

For USB version:

Put the board in Boot Loader mode (JP1)

stm32flash -v -w bin/mmdvm_f4.bin /dev/ttyUSB0


For PiHat:

stm32flash -v -w bin/mmdvm_f4.bin -g 0x0 -R -i 20,-21,21:-20,21 /dev/ttyAMA0

Note:
MUST be stm32flash version 0.5 or later 
