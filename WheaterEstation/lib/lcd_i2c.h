#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"


//#define I2C_PORT i2c0


// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0
#define DELAY_US 600

class LCDI2C
{

  private:
  	
	int address = 0x27; // By default these LCD display drivers are on bus address 0x27
	int maxLines = 2;
	int maxChars = 16;
	unsigned int pinNumSDA = 12;//4
	unsigned int pinNumSCL = 13;//5
	int i2cPortNumber = 0;

	// commands
	const int LCD_CLEARDISPLAY = 0x01;
	const int LCD_RETURNHOME = 0x02;
	const int LCD_ENTRYMODESET = 0x04;
	const int LCD_DISPLAYCONTROL = 0x08;
	const int LCD_CURSORSHIFT = 0x10;
	const int LCD_FUNCTIONSET = 0x20;
	const int LCD_SETCGRAMADDR = 0x40;
	const int LCD_SETDDRAMADDR = 0x80;

	// flags for display entry mode
	const int LCD_ENTRYSHIFTINCREMENT = 0x01;
	const int LCD_ENTRYLEFT = 0x02;

	// flags for display and cursor control
	const int LCD_BLINKON = 0x01;
	const int LCD_CURSORON = 0x02;
	const int LCD_DISPLAYON = 0x04;

	// flags for display and cursor shift
	const int LCD_MOVERIGHT = 0x04;
	const int LCD_DISPLAYMOVE = 0x08;

	// flags for function set
	const int LCD_5x10DOTS = 0x04;
	const int LCD_2LINE = 0x08;
	const int LCD_8BITMODE = 0x10;

	// flag for backlight control
	const int LCD_BACKLIGHT = 0x08;

	const int LCD_ENABLE_BIT = 0x04;
	
	void i2c_write_byte(uint8_t val);
	void lcd_toggle_enable(uint8_t val);
	void lcd_send_byte(uint8_t val, int mode);	
	void lcd_char(char val);	
	void lcd_init();
	
  public:
  	LCDI2C();
  	LCDI2C(int addr, int rows, int columns, unsigned int pinSDA, unsigned int pinSCL, int i2cPort);
  	void init();  	
  	void lcd_clear(void);
  	int getMaxRows();
	int getMaxColumns();
	void lcd_set_cursor(int line, int position);
	void lcd_string(const char *s);	
	
};




