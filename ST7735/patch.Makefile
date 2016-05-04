--- Makefile	2016-05-03 19:34:17.389615525 -0400
+++ Makefile	2016-05-01 13:22:04.000000000 -0400
@@ -3,19 +3,19 @@
 CC      = gcc
 CXX     = g++
 CFLAGS  = -g -O3 -Wall -std=c++0x
-LIBS    =
-LDFLAGS = -g
+LIBS    = -lbcm2835
+LDFLAGS = -g -L/usr/local/lib
 
 OBJECTS = \
 		AMBEFEC.o BPTC19696.o Conf.o CRC.o Display.o DMRControl.o DMRCSBK.o DMRData.o DMRDataHeader.o DMREMB.o DMREmbeddedLC.o DMRFullLC.o DMRIPSC.o DMRLookup.o DMRLC.o \
 		DMRShortLC.o DMRSlot.o DMRSlotType.o DStarControl.o DStarHeader.o DStarNetwork.o DStarSlowData.o Golay2087.o Golay24128.o Hamming.o Log.o MMDVMHost.o Modem.o \
 		Nextion.o NullDisplay.o QR1676.o RS129.o SerialController.o SHA256.o StopWatch.o Sync.o TFTSerial.o Timer.o UDPSocket.o Utils.o YSFControl.o YSFConvolution.o \
-		YSFFICH.o YSFParrot.o YSFPayload.o
+		YSFFICH.o YSFParrot.o YSFPayload.o st7735Display.o bitmap.o
 
 all:		MMDVMHost
 
 MMDVMHost:	$(OBJECTS)
-		$(CXX) $(OBJECTS) $(CFLAGS) $(LIBS) -o MMDVMHost
+		$(CXX) $(OBJECTS) $(CFLAGS) $(LIBS) $(LDFLAGS) -o MMDVMHost
 
 %.o: %.cpp
 		$(CXX) $(CFLAGS) -c -o $@ $<
