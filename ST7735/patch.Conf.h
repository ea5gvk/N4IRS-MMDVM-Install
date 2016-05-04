--- Conf.h	2016-05-03 19:34:17.369615525 -0400
+++ Conf.h	2016-05-01 16:53:56.000000000 -0400
@@ -36,7 +36,6 @@
   bool         getDuplex() const;
   unsigned int getModeHang() const;
   std::string  getDisplay() const;
-  bool         getDaemon() const;
 
   // The Info section
   unsigned int getRxFrequency() const;
@@ -118,16 +117,16 @@
   unsigned int getHD44780Rows() const;
   unsigned int getHD44780Columns() const;
   std::vector<unsigned int> getHD44780Pins() const;
-  bool         getHD44780PWM() const;
-  unsigned int getHD44780PWMPin() const;
-  unsigned int getHD44780PWMBright() const;
-  unsigned int getHD44780PWMDim() const;
 
   // The Nextion section
   std::string  getNextionSize() const;
   std::string  getNextionPort() const;
   unsigned int getNextionBrightness() const;
 
+  //ST7735
+  unsigned int getST7735DisplayNum() const;
+  std::string  getST7735BitmapPath() const;
+  
 private:
   std::string  m_file;
   std::string  m_callsign;
@@ -135,7 +134,6 @@
   bool         m_duplex;
   unsigned int m_modeHang;
   std::string  m_display;
-  bool         m_daemon;
 
   unsigned int m_rxFrequency;
   unsigned int m_txFrequency;
@@ -206,14 +204,14 @@
   unsigned int m_hd44780Rows;
   unsigned int m_hd44780Columns;
   std::vector<unsigned int> m_hd44780Pins;
-  bool         m_hd44780PWM;
-  unsigned int m_hd44780PWMPin;
-  unsigned int m_hd44780PWMBright;
-  unsigned int m_hd44780PWMDim;
 
   std::string  m_nextionSize;
   std::string  m_nextionPort;
   unsigned int m_nextionBrightness;
+  
+  unsigned int m_st7735DisplayNum;
+  std::string  m_st7735BitmapPath;
+
 };
 
 #endif
