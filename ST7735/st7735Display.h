// Created by n4irr on 12/16/2015.
// Copyright (c) n4irr All rights reserved.
//

#if !defined(ST7735DISPLAY_H)
#define	ST7735DISPLAY_H

#include "Display.h"
#include "Defines.h"

#include <string>

class CST7735Display : public IDisplay
{
public:
  CST7735Display(const std::string& callsign, unsigned int dmrid, const std::string& path, unsigned int displayNum);
  virtual ~CST7735Display();

  virtual bool open();

  virtual void setIdle();

  virtual void setError(const char* text);
  virtual void setLockout();

  virtual void writeDStar(const char* my1, const char* my2, const char* your, const char* type, const char* reflector);
  virtual void clearDStar();

  virtual void writeDMR(unsigned int slotNo, const std::string& src, bool group, const std::string& dst, const char* type);
  virtual void clearDMR(unsigned int slotNo);

  virtual void writeFusion(const char* source, const char* dest);
  virtual void clearFusion();

  virtual void close();

private:
   std::string       m_callsign;
   unsigned int      m_dmrid;
   std::string       m_path;
   unsigned int      m_displayNum;
   unsigned char     m_mode;

  void clearScreen();
  void setBackground(unsigned char colour);
  void setForeground(unsigned char colour);
  void setRotation(unsigned char rotation);
  void setFontSize(unsigned char size);
  void gotoBegOfLine();
  void gotoPosText(unsigned char x, unsigned char y);
  void gotoPosPixel(unsigned char x, unsigned char y);
  void drawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
  void drawBox(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, bool filled);
  void drawCircle(unsigned char x, unsigned char y, unsigned char radius, bool filled);
  void displayBitmap(unsigned char x, unsigned char y, const char* filename);
  void setBrightness(unsigned char brightness);
  void displayText(const char* text);
};

#endif
