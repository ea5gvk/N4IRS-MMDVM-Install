--- Conf.cpp	2016-05-03 19:34:17.369615525 -0400
+++ Conf.cpp	2016-05-01 18:10:27.000000000 -0400
@@ -41,7 +41,8 @@
   SECTION_FUSION_NETWORK,
   SECTION_TFTSERIAL,
   SECTION_HD44780,
-  SECTION_NEXTION
+  SECTION_NEXTION,
+  SECTION_ST7735
 };
 
 CConf::CConf(const std::string& file) :
@@ -51,7 +52,6 @@
 m_duplex(true),
 m_modeHang(10U),
 m_display(),
-m_daemon(false),
 m_rxFrequency(0U),
 m_txFrequency(0U),
 m_power(0U),
@@ -106,18 +106,16 @@
 m_fusionNetworkAddress(),
 m_fusionNetworkPort(0U),
 m_fusionNetworkDebug(false),
-m_tftSerialPort("/dev/ttyAMA0"),
+m_tftSerialPort(),
 m_tftSerialBrightness(50U),
 m_hd44780Rows(2U),
 m_hd44780Columns(16U),
 m_hd44780Pins(),
-m_hd44780PWM(false),
-m_hd44780PWMPin(),
-m_hd44780PWMBright(),
-m_hd44780PWMDim(),
-m_nextionSize("2.4"),
-m_nextionPort("/dev/ttyAMA0"),
-m_nextionBrightness(50U)
+m_nextionSize(),
+m_nextionPort(),
+m_nextionBrightness(50U),
+m_st7735DisplayNum(0),
+m_st7735BitmapPath("TFTSerial")
 {
 }
 
@@ -167,6 +165,8 @@
 		  section = SECTION_HD44780;
 	  else if (::strncmp(buffer, "[Nextion]", 9U) == 0)
 		  section = SECTION_NEXTION;
+	  else if (::strncmp(buffer, "[ST7735]", 8U) == 0)
+		  section = SECTION_ST7735;
 	  else
         section = SECTION_NONE;
 
@@ -192,8 +192,6 @@
 			m_modeHang = (unsigned int)::atoi(value);
 		else if (::strcmp(key, "Display") == 0)
 			m_display = value;
-		else if (::strcmp(key, "Daemon") == 0)
-			m_daemon = ::atoi(value) == 1;
 	} else if (section == SECTION_INFO) {
 		if (::strcmp(key, "TXFrequency") == 0)
 			m_txFrequency = (unsigned int)::atoi(value);
@@ -347,14 +345,6 @@
 			m_hd44780Rows = (unsigned int)::atoi(value);
 		else if (::strcmp(key, "Columns") == 0)
 			m_hd44780Columns = (unsigned int)::atoi(value);
-		else if (::strcmp(key, "PWM") == 0)
-			m_hd44780PWM = ::atoi(value) == 1;
-		else if (::strcmp(key, "PWMPin") == 0)
-			m_hd44780PWMPin = (unsigned int)::atoi(value);
-		else if (::strcmp(key, "PWMBright") == 0)
-			m_hd44780PWMBright = (unsigned int)::atoi(value);
-		else if (::strcmp(key, "PWMDim") == 0)
-			m_hd44780PWMDim = (unsigned int)::atoi(value);
 		else if (::strcmp(key, "Pins") == 0) {
 			char* p = ::strtok(value, ",\r\n");
 			while (p != NULL) {
@@ -363,6 +353,11 @@
 				p = ::strtok(NULL, ",\r\n");
 			}
 		}
+	} else if (section == SECTION_ST7735) {
+		if (::strcmp(key, "DisplayNum") == 0)
+			m_st7735DisplayNum = (unsigned int)::atoi(value);
+		if (::strcmp(key, "BitmapPath") == 0)
+			m_st7735BitmapPath = value;
 	} else if (section == SECTION_NEXTION) {
 		if (::strcmp(key, "Size") == 0)
 			m_nextionSize = value;
@@ -403,11 +398,6 @@
   return m_display;
 }
 
-bool CConf::getDaemon() const
-{
-	return m_daemon;
-}
-
 unsigned int CConf::getRxFrequency() const
 {
 	return m_rxFrequency;
@@ -703,26 +693,6 @@
 	return m_hd44780Pins;
 }
 
-bool CConf::getHD44780PWM() const
-{
-	return m_hd44780PWM;
-}
-
-unsigned int CConf::getHD44780PWMPin() const
-{
-	return m_hd44780PWMPin;
-}
-
-unsigned int CConf::getHD44780PWMBright() const
-{
-	return m_hd44780PWMBright;
-}
-
-unsigned int CConf::getHD44780PWMDim() const
-{
-	return m_hd44780PWMDim;
-}
-
 std::string CConf::getNextionSize() const
 {
 	return m_nextionSize;
@@ -737,3 +707,17 @@
 {
 	return m_nextionBrightness;
 }
+
+unsigned int CConf::getST7735DisplayNum() const
+{
+	return m_st7735DisplayNum;
+}
+
+std::string  CConf::getST7735BitmapPath() const
+{
+	if (m_st7735BitmapPath.c_str() != NULL)
+	{
+		return m_st7735BitmapPath;
+	}
+	return "TFTSerial";
+}
