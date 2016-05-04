--- MMDVMHost.cpp	2016-05-03 19:34:17.389615525 -0400
+++ MMDVMHost.cpp	2016-05-01 17:35:04.000000000 -0400
@@ -27,6 +27,7 @@
 #include "NullDisplay.h"
 #include "YSFControl.h"
 #include "Nextion.h"
+#include "st7735Display.h"
 
 #if defined(HD44780)
 #include "HD44780.h"
@@ -38,53 +39,38 @@
 #if !defined(_WIN32) && !defined(_WIN64)
 #include <unistd.h>
 #include <signal.h>
-#include <fcntl.h>
-#endif
-
-#if defined(_WIN32) || defined(_WIN64)
-const char* DEFAULT_INI_FILE = "mmdvm.ini";
-#else
-const char* DEFAULT_INI_FILE = "/etc/mmdvm.ini";
 #endif
 
 static bool m_killed = false;
-static int  m_signal = 0;
 
 #if !defined(_WIN32) && !defined(_WIN64)
-static void sigHandler(int signum)
+static void sigHandler(int)
 {
   m_killed = true;
-  m_signal = signum;
 }
 #endif
 
 const char* HEADER1 = "This software is for use on amateur radio networks only,";
 const char* HEADER2 = "it is to be used for educational purposes only. Its use on";
 const char* HEADER3 = "commercial networks is strictly prohibited.";
-const char* HEADER4 = "Copyright(C) 2015, 2016 by Jonathan Naylor, G4KLX and others";
+const char* HEADER4 = "Copyright(C) 2015, 2016 by Jonathan Naylor, G4KLX";
 
 int main(int argc, char** argv)
 {
-  const char* iniFile = DEFAULT_INI_FILE;
-  if (argc > 1)
-	iniFile = argv[1];
+  if (argc == 1) {
+    ::fprintf(stderr, "Usage: MMDVMHost <conf file>\n");
+    return 1;
+  }
 
 #if !defined(_WIN32) && !defined(_WIN64)
-  ::signal(SIGTERM, sigHandler);
-  ::signal(SIGHUP,  sigHandler);
+  ::signal(SIGUSR1, sigHandler);
 #endif
 
-  CMMDVMHost* host = new CMMDVMHost(std::string(iniFile));
+  CMMDVMHost* host = new CMMDVMHost(std::string(argv[1]));
   int ret2 = host->run();
 
   delete host;
 
-  if (m_signal == 15)
-	  ::LogInfo("Caught SIGTERM. Exiting");
-
-  if (m_signal == 1)
-	  ::LogInfo("Caught SIGHUP. Exiting");
-
   ::LogFinalise();
 
   return ret2;
@@ -123,30 +109,6 @@
 		return 1;
 	}
 
-#if !defined(_WIN32) && !defined(_WIN64)
-	bool m_daemon = m_conf.getDaemon();
-	if (m_daemon) {
-		// Create new process
-		pid_t pid = ::fork();
-		if (pid == -1)
-			return -1;
-		else if (pid != 0)
-			exit(EXIT_SUCCESS);
-
-		// Create new session and process group
-		if (::setsid() == -1)
-			return -1;
-
-		// Set the working directory to the root directory
-		if (::chdir("/") == -1)
-			return -1;
-
-		::close(STDIN_FILENO);
-		::close(STDOUT_FILENO);
-		::close(STDERR_FILENO);
-	}
-#endif
-
 	LogInfo(HEADER1);
 	LogInfo(HEADER2);
 	LogInfo(HEADER3);
@@ -607,9 +569,9 @@
 
 void CMMDVMHost::createDisplay()
 {
-	std::string type       = m_conf.getDisplay();
-	std::string callsign   = m_conf.getCallsign();
-	unsigned int dmrid     = m_conf.getDMRId();
+	std::string type     = m_conf.getDisplay();
+	std::string callsign = m_conf.getCallsign();
+	unsigned int dmrid   = m_conf.getDMRId();
 
 	LogInfo("Display Parameters");
 	LogInfo("    Type: %s", type.c_str());
@@ -637,26 +599,23 @@
 		unsigned int rows    = m_conf.getHD44780Rows();
 		unsigned int columns = m_conf.getHD44780Columns();
 		std::vector<unsigned int> pins = m_conf.getHD44780Pins();
-		bool pwm = m_conf.getHD44780PWM();
-		unsigned int pwmPin = m_conf.getHD44780PWMPin();
-		unsigned int pwmBright = m_conf.getHD44780PWMBright();
-		unsigned int pwmDim = m_conf.getHD44780PWMDim();
 
 		if (pins.size() == 6U) {
 			LogInfo("    Rows: %u", rows);
 			LogInfo("    Columns: %u", columns);
 			LogInfo("    Pins: %u,%u,%u,%u,%u,%u", pins.at(0U), pins.at(1U), pins.at(2U), pins.at(3U), pins.at(4U), pins.at(5U));
 
-			if (pwm) {
-				LogInfo("PWM Brightness Control Enabled");
-				LogInfo("    PWM Pin: %u", pwmPin);
-				LogInfo("    PWM Bright: %u", pwmBright);
-				LogInfo("    PWM Dim: %u", pwmDim);
-			}
-
-			m_display = new CHD44780(rows, columns, callsign, dmrid, pins, pwm, pwmPin, pwmBright, pwmDim);
+			m_display = new CHD44780(rows, columns, callsign, dmrid, pins);
 		}
 #endif
+
+	} else if (type == "ST7735") {
+		unsigned int displayNum = m_conf.getST7735DisplayNum();
+		std::string path        = m_conf.getST7735BitmapPath();
+		LogInfo("    DisplayNum: %u", displayNum);
+		LogInfo("    BitmapPath: <%s>", path.c_str());
+
+		m_display = new CST7735Display(callsign, dmrid, path, displayNum);
 	} else {
 		m_display = new CNullDisplay;
 	}
