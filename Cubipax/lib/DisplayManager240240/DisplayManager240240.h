#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <Pictures.h>
#include <SD.h>
/*
#define TFT_CS1   12  // Chip select control pin
#define TFT_CS2   14  // Chip select control pin
#define TFT_CS3   13  // Chip select control pin
#define TFT_CS4   22  // Chip select control pin
#define TFT_CS5   33 // Chip select control pin
#define TFT_CS6   16  // Chip select control pin
*/

#define BUFF_SIZE 240

//************************LIS3DH
// Used for software SPI
//#define LIS3DH_CLK 18
//#define LIS3DH_MISO 19
//#define LIS3DH_MOSI 23
// Used for hardware & software SPI
//#define LIS3DH_CS 15



//************************
void Setup_SPIs(int ChipSelect);
void Write_LCD1(const String &InputStr);//void Write_LCD1(char *InputStr);
void Write_LCD2(const String &InputStr);
void Write_LCD3(const String &InputStr);
void Write_LCD4(const String &InputStr);
void Write_LCD5(const String &InputStr);
void Write_LCD6(const String &InputStr);
void LCD_Init();
void drawIcon(const unsigned short* icon, int16_t x, int16_t y,  uint16_t width, uint16_t height);
void bmpDraw(char *filename, uint8_t x, uint16_t y);
void SDcheck();
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);