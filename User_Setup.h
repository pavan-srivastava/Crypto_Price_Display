#define ST7789_DRIVER

#define TFT_MOSI 23
// #define TFT_MISO 19 -- Not needed
#define TFT_SCLK 18
#define TFT_CS   5 // Chip select control pin
#define TFT_DC   21  // Data Command control pin
#define TFT_RST  22  // Reset pin (could connect to RST pin)

// Optional - define for XPT2046 touch controller
//#define TOUCH_CS 22     // Chip select pin (T_CS) of touch screen

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Comment out the #define below to stop the SPIFFS filing system and smooth font code being loaded
// this will save ~20kbytes of FLASH
#define SMOOTH_FONT

// #define SPI_FREQUENCY  27000000 // Actually sets it to 26.67MHz = 80/3
#define SPI_FREQUENCY  40000000 // Maximum recommended for ILI9341

// Optional reduced SPI frequency for reading TFT
// #define SPI_READ_FREQUENCY  10000000

// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY  400000
