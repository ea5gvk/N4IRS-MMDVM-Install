// hy18spi.c
//
// Example program for HY-1.8 SPI LCD using bcm2835 library.
// Shows how to interface with SPI and draws couple of patterns.
//
// After installing bcm2835, you can build it with this command
// gcc -o hy18spi hy18spi.c -l bcm2835
// and then test
// sudo ./hy18spi
//
// Or you can you can modify Makefile
//
// Detailed Raspberry Pi GPIO info can be found here:
// http://www.combinatorialdesign.com/boards/Raspberry_Pi/
//
// Author: Pawel Wodnicki
// Copyright (C) 2013 Pawel Wodnicki
// $Id: $

/*
 Most of this code has been borrowed from ST7735 frame
 buffer driver in drivers/video/st7735 and some form
 bcm2835 library spi example.
 This example is licensed under GPL V2 same as bcm2835 library.
 
 */

/*
 HY-1.8 SPI connections
 
 HY-1.8 J1        Raspberry Pi P1      bcm2835 lib pin
 1 - GND          - P1.6 GND
 2 - VCC          - P1.2 5V Power
 3 - NC
 4 - NC
 5 - NC
 6 - RESET        - P1.22 GPIO25        RPI_V2_GPIO_P1_22
 7 - A0           - P1.18 GPIO24        RPI_V2_GPIO_P1_18
 8 - SDA          - P1.19 GPIO10 MOSI   RPI_V2_GPIO_P1_19
 9 - SCK          - P1.23 GPIO11 SCLK   RPI_V2_GPIO_P1_23
 10 - CS          - P1.24 GPIO8 CE0     RPI_V2_GPIO_P1_24
 11 - SD_SCK
 12 - SD_MISO
 13 - SD_MOSI
 14 - SD_CS
 15 - LED+         - P1.4 5V Power
 16 - LED-         - P1.9 GND
 
 */

#define RPI_GPIO_A0	RPI_BPLUS_GPIO_J8_31
#define RPI_GPIO_SDA	RPI_BPLUS_GPIO_J8_19
#define RPI_GPIO_RST	RPI_BPLUS_GPIO_J8_22
#define RPI_GPIO_CS	RPI_BPLUS_GPIO_J8_24

#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include "st7735.h"
#include "progfont.h"

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

// forward declarations
void hy18spi_init_gpio(void);
void hy18spi_end_gpio(void);
void hy18spi_reset();
void hy18spi_init(void);
void hy18spi_end(void);
uint8_t hy18spi_transfer_cmd(uint8_t value);
uint8_t hy18spi_transfer_data(uint8_t value);
void hy18spi_probe();
void hy18spi_set_rgb(uint16_t color);
void hy18spi_set_pixel(uint16_t x, uint16_t y, uint16_t color);
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void writeText(uint8_t c);
void setCursor(int16_t x, int16_t y);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void swap(int16_t a, int16_t b);
void printText(const char *str);

#define HY18SPI_WIDTH 160//128
#define HY18SPI_HEIGHT 128//160
//#define textcolor 0xffff
//#define textbgcolor 0x0000
#define wrap 1
//#define textsize 1

// configuration - comment to disable
#define CONFIG_FB_ST7735_PANEL_TYPE_RED_TAB 1
//#define CONFIG_FB_ST7735_RGB_ORDER_REVERSED 1

#if ( CONFIG_FB_ST7735_PANEL_TYPE_RED_TAB == 1 )
# define ST7735_COLSTART 0
# define ST7735_ROWSTART 0
#else
# define ST7735_COLSTART 2
# define ST7735_ROWSTART 1
#endif

int16_t cursor_y=0;
int16_t cursor_x=0;
int8_t textsize = 2;
uint16_t  textcolor = 0xffff;
uint16_t  textbgcolor = 0x0000;

/* Init script function */
struct st7735_function {
    uint16_t cmd;
    uint16_t data;
};

/* Init script commands */
enum st7735_cmd {
    ST7735_START,
    ST7735_END,
    ST7735_CMD,
    ST7735_DATA,
    ST7735_DELAY
};

static struct st7735_function st7735_cfg_script[] = {
    { ST7735_START, ST7735_START},
    { ST7735_CMD, ST7735_SWRESET},
    { ST7735_DELAY, 150},
    { ST7735_CMD, ST7735_SLPOUT},
    { ST7735_DELAY, 500},
    { ST7735_CMD, ST7735_FRMCTR1},
    { ST7735_DATA, 0x01},
    { ST7735_DATA, 0x2c},
    { ST7735_DATA, 0x2d},
    { ST7735_CMD, ST7735_FRMCTR2},
    { ST7735_DATA, 0x01},
    { ST7735_DATA, 0x2c},
    { ST7735_DATA, 0x2d},
    { ST7735_CMD, ST7735_FRMCTR3},
    { ST7735_DATA, 0x01},
    { ST7735_DATA, 0x2c},
    { ST7735_DATA, 0x2d},
    { ST7735_DATA, 0x01},
    { ST7735_DATA, 0x2c},
    { ST7735_DATA, 0x2d},
    { ST7735_CMD, ST7735_INVCTR},
    { ST7735_DATA, 0x07},
    { ST7735_CMD, ST7735_PWCTR1},
    { ST7735_DATA, 0xa2},
    { ST7735_DATA, 0x02},
    { ST7735_DATA, 0x84},
    { ST7735_CMD, ST7735_PWCTR2},
    { ST7735_DATA, 0xc5},
    { ST7735_CMD, ST7735_PWCTR3},
    { ST7735_DATA, 0x0a},
    { ST7735_DATA, 0x00},
    { ST7735_CMD, ST7735_PWCTR4},
    { ST7735_DATA, 0x8a},
    { ST7735_DATA, 0x2a},
    { ST7735_CMD, ST7735_PWCTR5},
    { ST7735_DATA, 0x8a},
    { ST7735_DATA, 0xee},
    { ST7735_CMD, ST7735_VMCTR1},
    { ST7735_DATA, 0x0e},
    { ST7735_CMD, ST7735_INVOFF},
    { ST7735_CMD, ST7735_MADCTL},
#if ( CONFIG_FB_ST7735_RGB_ORDER_REVERSED == 1 )
    { ST7735_DATA, 0xc0},
#else
    { ST7735_DATA, 0xc8},
#endif
    { ST7735_CMD, ST7735_COLMOD},
    { ST7735_DATA, 0x05},
#if 0 /* set_addr_win will set these, so no need to set them at init */
    { ST7735_CMD, ST7735_CASET},
    { ST7735_DATA, 0x00},
    { ST7735_DATA, 0x00 + ST7735_COLSTART},
    { ST7735_DATA, 0x00},
    { ST7735_DATA, HY18SPI_WIDTH - 1 + ST7735_COLSTART},
    { ST7735_CMD, ST7735_RASET},
    { ST7735_DATA, 0x00},
    { ST7735_DATA, 0x00 + ST7735_ROWSTART},
    { ST7735_DATA, 0x00},
    { ST7735_DATA, HY18SPI_HEIGHT - 1 + ST7735_ROWSTART},
#endif
    { ST7735_CMD, ST7735_GMCTRP1},
    { ST7735_DATA, 0x02},
    { ST7735_DATA, 0x1c},
    { ST7735_DATA, 0x07},
    { ST7735_DATA, 0x12},
    { ST7735_DATA, 0x37},
    { ST7735_DATA, 0x32},
    { ST7735_DATA, 0x29},
    { ST7735_DATA, 0x2d},
    { ST7735_DATA, 0x29},
    { ST7735_DATA, 0x25},
    { ST7735_DATA, 0x2b},
    { ST7735_DATA, 0x39},
    { ST7735_DATA, 0x00},
    { ST7735_DATA, 0x01},
    { ST7735_DATA, 0x03},
    { ST7735_DATA, 0x10},
    { ST7735_CMD, ST7735_GMCTRN1},
    { ST7735_DATA, 0x03},
    { ST7735_DATA, 0x1d},
    { ST7735_DATA, 0x07},
    { ST7735_DATA, 0x06},
    { ST7735_DATA, 0x2e},
    { ST7735_DATA, 0x2c},
    { ST7735_DATA, 0x29},
    { ST7735_DATA, 0x2d},
    { ST7735_DATA, 0x2e},
    { ST7735_DATA, 0x2e},
    { ST7735_DATA, 0x37},
    { ST7735_DATA, 0x3f},
    { ST7735_DATA, 0x00},
    { ST7735_DATA, 0x00},
    { ST7735_DATA, 0x02},
    { ST7735_DATA, 0x10},
    ///#if 0 /* init_display will turn on the display after clearing it */
    { ST7735_CMD, ST7735_DISPON},
    { ST7735_DELAY, 100},
    ///#endif
    { ST7735_CMD, ST7735_NORON},
    { ST7735_DELAY, 10},
    { ST7735_END, ST7735_END},
};

void startTft() {
    //unsigned int pin;
    //int x,y;
    if (!bcm2835_init())
        return;
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);                   // MODE 3
    ///    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    /// NOT OK    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4); //< 4 = 16ns = 62.5MHz
    /// OK
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8); //< 8 = 32ns = 31.25MHz
    /// OK   bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16); //< 16 = 64ns = 15.625MHz
    /// OK   bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32); //< 32 = 128ns = 7.8125MHz
    /// OK   bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); //< 64 = 256ns = 3.90625MHz - 4000000 spi clock
    /// OK   bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    hy18spi_init_gpio();
    hy18spi_init();
    bcm2835_spi_chipSelect(BCM2835_SPI_CS1);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);      // the default
    hy18spi_init_gpio();
    hy18spi_init();
}

void screenSelect( int screen ) {
    if (screen == 0) {
        bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
        bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    }
    else
    {
        bcm2835_spi_chipSelect(BCM2835_SPI_CS1);                      // The default
        bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);      // the default
    }
}

void printText(const char *str) {
    while(*str)
        writeText(*str++);
}

// hardware reset
void hy18spi_reset()
{
    bcm2835_gpio_clr(RPI_GPIO_RST); //GPIO25 - RESET
    delay(10);
    bcm2835_gpio_set(RPI_GPIO_RST); //GPIO25 - RESET
    delay(120);
}

void hy18spi_init_gpio()
{
    // SPI pins have been initialized including GPIO8 - CS
    // A0 and RESET pins set to output
    bcm2835_gpio_fsel(RPI_GPIO_A0, BCM2835_GPIO_FSEL_OUTP); // A0 - GPIO24
    bcm2835_gpio_fsel(RPI_GPIO_RST, BCM2835_GPIO_FSEL_OUTP); // RESET - GPIO25
    // A0 set high
    bcm2835_gpio_set(RPI_GPIO_A0); //GPIO24 - A0
    // RESET set high
    bcm2835_gpio_set(RPI_GPIO_RST); //GPIO25 - RESET
    // CS set high
    bcm2835_gpio_set(RPI_GPIO_CS); //GPIO8 - CS
    
}

// gpio deinit
void hy18spi_end_gpio()
{
    // A0 and RESET pins set to input
    bcm2835_gpio_fsel(RPI_GPIO_A0, BCM2835_GPIO_FSEL_INPT); // A0 - GPIO24
    bcm2835_gpio_fsel(RPI_GPIO_RST, BCM2835_GPIO_FSEL_INPT); // RESET - GPIO25
}

uint8_t hy18spi_transfer_cmd(uint8_t value)
{
    bcm2835_gpio_clr(RPI_GPIO_A0); //GPIO24 - A0
    return bcm2835_spi_transfer(value);
}

uint8_t hy18spi_transfer_data(uint8_t value)
{
    bcm2835_gpio_set(RPI_GPIO_A0); //GPIO24 - A0
    return bcm2835_spi_transfer(value);
}

void hy18spi_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    hy18spi_transfer_cmd(ST7735_CASET);         // column addr set
    hy18spi_transfer_data(0x00);
    hy18spi_transfer_data(x0+ST7735_COLSTART);  // XSTART
    hy18spi_transfer_data(0x00);
    hy18spi_transfer_data(x1+ST7735_COLSTART);  // XEND
    
    hy18spi_transfer_cmd(ST7735_RASET);         // row addr set
    hy18spi_transfer_data(0x00);
    hy18spi_transfer_data(y0+ST7735_ROWSTART);  // YSTART
    hy18spi_transfer_data(0x00);
    hy18spi_transfer_data(y1+ST7735_ROWSTART);  // YEND
}

void hy18spi_set_rgb(uint16_t color)
{
    uint8_t x, y;
    hy18spi_set_addr_window(0, 0, HY18SPI_HEIGHT - 1, HY18SPI_WIDTH - 1);
    hy18spi_transfer_cmd(ST7735_RAMWR);  // write to RAM
    for (x=0; x < HY18SPI_HEIGHT; x++)
    {
        for (y=0; y < HY18SPI_WIDTH; y++)
        {
            hy18spi_transfer_data(color >> 8);
            hy18spi_transfer_data(color);
        }
    }
    hy18spi_transfer_cmd(ST7735_NOP);
}

void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {
    if((x >= HY18SPI_WIDTH) || (y >= HY18SPI_HEIGHT) || ((x + 6*size-1)<0) || ((y + 8 *size-1<0)))
        return;
    int8_t i;
    int8_t  j;
    for (i=0;i<6;i++) {
        uint8_t line;
        if (i==5)
            line=0x0;
        else
            line=pgm_read_byte(font+(c*5)+i);
        for (j = 0; j<8; j++) {
            if (line & 0x1) {
                if (size == 1) // default size
                    hy18spi_set_pixel(x+i, y+j, color);
                else { // big size
                    fillRect(x+i*size, y+j*size, size, size, color);
                }
            } else if (bg != color) {
                if (size == 1) // default size
                    hy18spi_set_pixel(x+i, y+j, bg);
                else { // big size
                    fillRect(x+i*size, y+j*size, size, size, bg);
                }
            }
            line >>= 1;
        }
    }
}

void writeText(uint8_t c) {
    if (c == '\n') {
        cursor_y += textsize*8;
        cursor_x = 0;
    } else if (c == '\r') {
        // skip em
    } else {
        drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
        cursor_x += textsize*6;
        if (wrap && (cursor_x > (HY18SPI_WIDTH - textsize*6))) {
            cursor_y += textsize*8;
            cursor_x = 0;
        }
    }
}

void setCursor(int16_t x, int16_t y) {
    cursor_x = x;
    cursor_y = y;
}

void hy18spi_set_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    uint16_t tmp = y;
    y = HY18SPI_WIDTH - x;
    x = tmp;
    hy18spi_set_addr_window(x,y,x+1,y+1);
    hy18spi_transfer_cmd(ST7735_RAMWR);  // write to RAM
    hy18spi_transfer_data(color >> 8);
    hy18spi_transfer_data(color);
}

static void hy18spi_run_cfg_script()
{
    int i = 0;
    int end_script = 0;
    
    do {
        switch (st7735_cfg_script[i].cmd)
        {
            case ST7735_START:
                break;
            case ST7735_CMD:
                hy18spi_transfer_cmd(st7735_cfg_script[i].data & 0xff);
                break;
            case ST7735_DATA:
                hy18spi_transfer_data(st7735_cfg_script[i].data & 0xff);
                break;
            case ST7735_DELAY:
                delay(st7735_cfg_script[i].data);
                break;
            case ST7735_END:
                end_script = 1;
        }
        i++;
    } while (!end_script);
}
// hy-1.8 init
void hy18spi_init(void)
{
    hy18spi_reset();
    hy18spi_run_cfg_script();
}

// hy-1.8 deinit
void hy18spi_end(void)
{
    hy18spi_end_gpio();
}


//Bresenham Algorythm
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */
    
    for(;;){  /* loop */
        hy18spi_set_pixel(x0,y0, color);
        if (x0==x1 && y0==y1) break;
        e2 = 2*err;
        if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    // Update in subclasses if desired!
    drawLine(x, y, x, y+h-1, color);
}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    // Update in subclasses if desired!
    drawLine(x, y, x+w-1, y, color);
}

#ifdef OLD_FILLRECT
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    // Update in subclasses if desired!
    int16_t i;
    for (i=x; i<x+w; i++) {
        drawFastVLine(i, y, h, color);
    }
}

#else

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    //Swap and rebase origin for rotated display
    int tx = HY18SPI_WIDTH-x;
    x = y;
    y = tx;
    
    for (;w>0;w--,y--)   //rotated: fill with color left to right (column)
    {
        int z = h;
        hy18spi_set_addr_window(x,y,x+h,y+1);
        hy18spi_transfer_cmd(ST7735_RAMWR);  // write to RAM
        while (z--) //rotated: fill with color top to bottom
        {
            hy18spi_transfer_data(color >> 8);
            hy18spi_transfer_data(color);
        }
    }
    hy18spi_transfer_cmd(ST7735_NOP);
}
#endif

void swap(int16_t a, int16_t b) {
    int16_t t=a;
    a=b;
    b=t;
}

uint16_t rgb_to_packed( uint32_t rgb )
{
    uint8_t r = (rgb >> 16) & 0xff;
    uint8_t g = (rgb >> 8) & 0xff;
    uint8_t b = (rgb & 0xff);
    return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
    
}

void ST7735_DrawBitmap(int16_t x, int16_t y, const uint8_t *image, int16_t w, int16_t h){
    int16_t skipC = 0;                      // non-zero if columns need to be skipped due to clipping
    int16_t originalWidth = w;              // save this value; even if not all columns fit on the screen, the image is still this width in ROM
    int i = w*(h - 1);
    
    int16_t _width = HY18SPI_WIDTH;
    int16_t _height = HY18SPI_HEIGHT;
    
    if((x >= _width) || ((y - h + 1) >= _height) || ((x + w) <= 0) || (y < 0)){
        printf("x = %d y = %d w = %d h = %d\n", x,y,w,h);
        printf("image is totally off the screen, do nothing\n");
        return;                             // image is totally off the screen, do nothing
    }
    if((w > _width) || (h > _height)){    // image is too wide for the screen, do nothing
        //***This isn't necessarily a fatal error, but it makes the
        //following logic much more complicated, since you can have
        //an image that exceeds multiple boundaries and needs to be
        //clipped on more than one side.
        printf("image is too wide for the screen, do nothing\n");
        return;
    }
    if((x + w - 1) >= _width){            // image exceeds right of screen
        skipC = (x + w) - _width;           // skip cut off columns
        w = _width - x;
        printf("image exceeds right of screen\n");
    }
    if((y - h + 1) < 0){                  // image exceeds top of screen
        i = i - (h - y - 1)*originalWidth;  // skip the last cut off rows
        h = y + 1;
        printf("image exceeds top of screen\n");
    }
    if(x < 0){                            // image exceeds left of screen
        w = w + x;
        skipC = -1*x;                       // skip cut off columns
        i = i - x;                          // skip the first cut off columns
        x = 0;
        printf("image exceeds left of screen\n");
    }
    if(y >= _height){                     // image exceeds bottom of screen
        h = h - (y - _height + 1);
        y = _height - 1;
        printf("image exceeds bottom of screen\n");
    }
    
    //    hy18spi_set_addr_window(x, y-h+1, x+w-1, y);
    
    for(y=0; y<h; y=y+1){
        for(x=0; x<w; x=x+1){
            int j = i*3;
            uint32_t packed = rgb_to_packed((image[j] << 16) | (image[j+1] << 8) | (image[j+2]));
            hy18spi_set_pixel(x, y, packed);
            //printf("plot %d %d\n",x,y);
            
            // send the top 8 bits
            //            hy18spi_transfer_data((uint8_t)(image[i] >> 8));
            // send the bottom 8 bits
            //            hy18spi_transfer_data((uint8_t)image[i]);
            i = i + 1;                        // go to the next pixel
        }
        i = i + skipC;
        i = i - 2*originalWidth;
    }
}
