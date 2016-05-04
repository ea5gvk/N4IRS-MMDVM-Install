
//
// testapp.c
// TFT test harness
//
// Created by n4irr on 12/16/2015.
// Copyright (c) n4irr All rights reserved.
//

#include "st7735Display.h"
#include "Log.h"
#include <cstdio>
#include <cassert>
#include <cstring>
extern "C" {
#include "hy18spi.h"
#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "bitmap.h"
#include <assert.h>
}

const unsigned char ROTATION_PORTRAIT_LEFT  = 0U;
const unsigned char ROTATION_LANDSCAPE_UD   = 1U;
const unsigned char ROTATION_PORTRAIT_RIGHT = 2U;
const unsigned char ROTATION_LANDSCAPE      = 3U;

const unsigned char COLOUR_BLACK   = 0U;
const unsigned char COLOUR_BLUE    = 1U;
const unsigned char COLOUR_RED     = 2U;
const unsigned char COLOUR_GREEN   = 3U;
const unsigned char COLOUR_CYAN    = 4U;
const unsigned char COLOUR_MAGENTA = 5U;
const unsigned char COLOUR_YELLOW  = 6U;
const unsigned char COLOUR_WHITE   = 7U;

const unsigned char FONT_SMALL  = 1U;
const unsigned char FONT_MEDIUM = 2U;
const unsigned char FONT_LARGE  = 3U;

// x = 0 to 159, y = 0 to 127 - Landscape
// x = 0 to 127, y = 0 to 159 - Portrait

static uint32_t colorTable[] = {0x000000, 0x0000ff, 0xff0000, 0x00ff00, 0x00ffff, 0xff00ff, 0xffff00, 0xffffff}; //rgb

uint16_t rgb_to_packed( uint32_t rgb );
extern int8_t textsize;
extern uint16_t textcolor;
extern uint16_t textbgcolor;

int fgColor = 0;
int bgColor = 0;
int cursorX = 0;
int cursorY = 0;

void initDisplay() {
    startTft();
    // set black background
    hy18spi_set_rgb(0x0000);
}

void deinitDisplay() {
    // deinitialize display
    hy18spi_end();
    
    // deintialize spi
    bcm2835_spi_end();
    
    // deintialize bcm2835 library
    bcm2835_close();
}

void drawBitmap(int16_t xorg, int16_t yorg, const uint8_t *image, int16_t w, int16_t h)
{
    int x,y;
    int i = 0;
    for(y=0; y<h; y=y+1){
        for(x=0; x<w; x=x+1){
            int j = i*3;
            uint32_t packed = rgb_to_packed((image[j] << 16) | (image[j+1] << 8) | (image[j+2]));
            hy18spi_set_pixel(xorg+x, yorg+h-y, packed);    //bitmaps are vertically flipped
            i = i + 1;                        // go to the next pixel
        }
    }
    
}

void printCenter( int line, const char *text )
{
	int len = strlen(text);
	int pixels = (len * textsize * 6);
	int padding = (160 - pixels) / 2;
	setCursor(padding, line);
    printText(text);
}

CST7735Display::CST7735Display(const std::string& callsign, unsigned int dmrid, const std::string& path, unsigned int displayNum) :
m_callsign(callsign),
m_dmrid(dmrid),
m_path(path),
m_displayNum(displayNum),
m_mode(MODE_IDLE)
{
	assert(displayNum >= 0U && displayNum <= 1U);
}

CST7735Display::~CST7735Display()
{
}

bool CST7735Display::open()
{
    initDisplay();
    screenSelect(m_displayNum);

	setRotation(ROTATION_LANDSCAPE);

	setBrightness(100);

	setBackground(COLOUR_WHITE);

	setForeground(COLOUR_BLACK);

	setIdle();

	return true;
}

void CST7735Display::setIdle()
{
	// Clear the screen
	clearScreen();

	setFontSize(FONT_MEDIUM);

	// Draw MMDVM logo
	displayBitmap(0U, 0U, "MMDVM_sm.bmp");

	char text[30];
	::sprintf(text, "%-6s/%u", m_callsign.c_str(), m_dmrid);

	printCenter(55U, text);
	printCenter(90U, "IDLE");

	m_mode = MODE_IDLE;
}

void CST7735Display::setError(const char* text)
{
	assert(text != NULL);

	// Clear the screen
	clearScreen();

	setFontSize(FONT_MEDIUM);

	// Draw MMDVM logo
	displayBitmap(0U, 0U, "MMDVM_sm.bmp");

	setForeground(COLOUR_RED);

	printCenter(55U, text);
	printCenter(90U, "ERROR");

	setForeground(COLOUR_BLACK);

	m_mode = MODE_ERROR;
}

void CST7735Display::setLockout()
{
	// Clear the screen
	clearScreen();

	setFontSize(FONT_LARGE);

	// Draw MMDVM logo
	displayBitmap(0U, 0U, "MMDVM_sm.bmp");

	printCenter(60U, "LOCKOUT");

	m_mode = MODE_LOCKOUT;
}

void CST7735Display::writeDStar(const char* my1, const char* my2, const char* your, const char* type, const char* reflector)
{
	assert(my1 != NULL);
	assert(my2 != NULL);
	assert(your != NULL);
	assert(type != NULL);
	assert(reflector != NULL);

	if (m_mode != MODE_DSTAR) {
		// Clear the screen
		clearScreen();

		setFontSize(FONT_MEDIUM);

		// Draw D-Star insignia
		displayBitmap(0U, 0U, "DStar_sm.bmp");
	}

	char text[30U];

	::sprintf(text, "%s %.8s/%4.4s", type, my1, my2);
	gotoPosPixel(5U, 70U);
	displayText(text);

	::sprintf(text, "%.8s", your);
	gotoPosPixel(5U, 90U);
	displayText(text);

	if (::strcmp(reflector, "        ") != 0) {
		::sprintf(text, "via %.8s", reflector);
		gotoPosPixel(5U, 110U);
		displayText(text);
	}

	m_mode = MODE_DSTAR;
}

void CST7735Display::clearDStar()
{
	gotoPosPixel(5U, 70U);
	displayText("  Listening  ");

	gotoPosPixel(5U, 90U);
	displayText("         ");

	gotoPosPixel(5U, 110U);
	displayText("             ");
}

void CST7735Display::writeDMR(unsigned int slotNo, const std::string& src, bool group, const std::string& dst, const char* type)
{
	assert(type != NULL);

	if (m_mode != MODE_DMR) {
		// Clear the screen
		clearScreen();

		setFontSize(FONT_MEDIUM);

		// Draw DMR insignia
		displayBitmap(0U, 0U, "DMR_sm.bmp");

		if (slotNo == 1U) {
			gotoPosPixel(5U, 90U);
			displayText("2 Listening");
		} else {
			gotoPosPixel(5U, 55U);
			displayText("1 Listening");
		}
	}

	if (slotNo == 1U) {
		char text[30U];

		::sprintf(text, "1 %s %-8s", type, src.c_str());
		gotoPosPixel(5U, 55U);
		displayText(text);

		::sprintf(text, "%s%s", group ? "TG" : "", dst.c_str());
		gotoPosPixel(65U, 72U);
		displayText(text);
	} else {
		char text[30U];

		::sprintf(text, "2 %s %-8s", type, src.c_str());
		gotoPosPixel(5U, 90U);
		displayText(text);

		::sprintf(text, "%s%s", group ? "TG" : "", dst.c_str());
		gotoPosPixel(65U, 107U);
		displayText(text);
	}

	m_mode = MODE_DMR;
}

void CST7735Display::clearDMR(unsigned int slotNo)
{
	if (slotNo == 1U) {
		gotoPosPixel(5U, 55U);
		displayText("1 Listening ");

		gotoPosPixel(65U, 72U);
		displayText("       ");
	} else {
		gotoPosPixel(5U, 90U);
		displayText("2 Listening ");

		gotoPosPixel(65U, 107U);
		displayText("       ");
	}
}

void CST7735Display::writeFusion(const char* source, const char* dest)
{
	assert(source != NULL);
	assert(dest != NULL);

	if (m_mode != MODE_YSF) {
		// Clear the screen
		clearScreen();

		setFontSize(FONT_MEDIUM);

		// Draw the System Fusion insignia
		displayBitmap(0U, 0U, "YSF_sm.bmp");
	}

	char text[30U];
	::sprintf(text, "%.10s", source);

	gotoPosPixel(5U, 80U);
	displayText(text);

	::sprintf(text, "%.10s", dest);

	gotoPosPixel(5U, 100U);
	displayText(text);

	m_mode = MODE_YSF;
}

void CST7735Display::clearFusion()
{
	gotoPosPixel(5U, 80U);
	displayText("  Listening ");

	gotoPosPixel(5U, 100U);
	displayText("           ");
}

void CST7735Display::close()
{
    deinitDisplay();
}

void CST7735Display::clearScreen()
{
    hy18spi_set_rgb(bgColor);
}

void CST7735Display::setForeground(unsigned char colour)
{
	assert(colour >= 0U && colour <= 7U);

    fgColor = rgb_to_packed(colorTable[colour]);
    textcolor = fgColor;
}

void CST7735Display::setBackground(unsigned char colour)
{
	assert(colour >= 0U && colour <= 7U);

    bgColor = rgb_to_packed(colorTable[colour]);
    textbgcolor = bgColor;
}

void CST7735Display::setRotation(unsigned char rotation)
{
	assert(rotation >= 0U && rotation <= 3U);

}

void CST7735Display::setFontSize(unsigned char size)
{
	assert(size >= 1U && size <= 3U);

    textsize = size;
}

void CST7735Display::gotoBegOfLine()
{
    cursorX = 0;
}

void CST7735Display::gotoPosText(unsigned char x, unsigned char y)
{
    setCursor( (x*6*textsize)+1, y*8*textsize );
}

void CST7735Display::gotoPosPixel(unsigned char x, unsigned char y)
{
    setCursor( x, y );
}

void CST7735Display::drawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    ::drawLine( x1, y1, x2, y2, fgColor);
}

void CST7735Display::drawBox(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, bool filled)
{
    if (filled == 0)
    {
        drawFastHLine(x1, y1, x2-x1, fgColor);
        drawFastHLine(x1, y2, x2-x1, fgColor);
        drawFastVLine(x1, y1, y2-y1, fgColor);
        drawFastVLine(x2, y1, y2-y1, fgColor);
    }
    else
        fillRect(x1, y1, x2-x1, y2-y1, fgColor);
}

void CST7735Display::drawCircle(unsigned char x0, unsigned char y0, unsigned char radius, bool filled)
{
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0
    
    while( y <= x )
    {
        hy18spi_set_pixel( x + x0,  y + y0, fgColor); // Octant 1
        hy18spi_set_pixel( y + x0,  x + y0, fgColor); // Octant 2
        hy18spi_set_pixel(-x + x0,  y + y0, fgColor); // Octant 4
        hy18spi_set_pixel(-y + x0,  x + y0, fgColor); // Octant 3
        hy18spi_set_pixel(-x + x0, -y + y0, fgColor); // Octant 5
        hy18spi_set_pixel(-y + x0, -x + y0, fgColor); // Octant 6
        hy18spi_set_pixel( x + x0, -y + y0, fgColor); // Octant 7
        hy18spi_set_pixel( y + x0, -x + y0, fgColor); // Octant 8
        y++;
        if (decisionOver2<=0)
        {
            decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
        }
        else
        {
            x--;
            decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
}

void CST7735Display::displayBitmap(unsigned char x, unsigned char y, const char* filename)
{
	assert(filename != NULL);

	char fnBuffer[128];
	::sprintf(fnBuffer, "%s/%s", m_path.c_str(), filename);
	
    BITMAPINFO *BitmapInfo; /* Bitmap information */
    GLubyte    *BitmapBits; /* Bitmap data */
    BitmapBits = LoadDIBitmap(fnBuffer, &BitmapInfo);
    if (BitmapBits)
    {
        drawBitmap(x, y, (const uint8_t *)BitmapBits, BitmapInfo->bmiHeader.biWidth, abs(BitmapInfo->bmiHeader.biHeight));
        free(BitmapBits);
        free(BitmapInfo);
    }
    else
        printf("no bitmap %s\n", filename);
}

void CST7735Display::setBrightness(unsigned char brightness)
{
	assert(brightness >= 0U && brightness <= 100U);

}

void CST7735Display::displayText(const char* text)
{
	assert(text != NULL);

    printText(text);
}




