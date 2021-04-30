#include "lcd_i2c.h"

LCDI2C::LCDI2C()
{
}

LCDI2C::LCDI2C(int addr, int rows, int columns, unsigned int pinSDA, unsigned int pinSCL, int i2cPort)
{
  address = addr;
  maxLines = rows;
  maxChars = columns;
  pinNumSDA = pinSDA;
  pinNumSCL = pinSCL;
  i2cPortNumber = i2cPort;
}

void LCDI2C::init()
{   
    if(i2cPortNumber == 1)
	    i2c_init(i2c1, 100 * 1000); 	
    else
	    i2c_init(i2c0, 100 * 1000);   
    
    
    gpio_set_function(pinNumSDA, GPIO_FUNC_I2C);
    gpio_set_function(pinNumSCL, GPIO_FUNC_I2C);
    gpio_pull_up(pinNumSDA);
    gpio_pull_up(pinNumSCL);
    // Make the I2C pins available to picotool
    bi_decl( bi_2pins_with_func(pinNumSDA, pinNumSCL, GPIO_FUNC_I2C));

    lcd_init();
}

/* Quick helper function for single byte transfers */
void LCDI2C::i2c_write_byte(uint8_t val) 
{
    if(i2cPortNumber == 1)
    	i2c_write_blocking(i2c1, address, &val, 1, false);
    else
    	i2c_write_blocking(i2c0, address, &val, 1, false);
}


void LCDI2C::lcd_toggle_enable(uint8_t val) 
{
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work

    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void LCDI2C::lcd_send_byte(uint8_t val, int mode) 
{
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void LCDI2C::lcd_clear(void) 
{
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

// go to location on LCD
void LCDI2C::lcd_set_cursor(int line, int position) 
{
    int val;
    
    if(line == 0)
    	val = 0x80 + position;
    else if(line == 1)
    	val = 0xC0 + position;
    else if (line == 2)
    	val = 0x94 + position;
    else if (line == 3)
    	val = 0xD4 + position;
    else
    	val = 0x80 + position;
    
    lcd_send_byte(val, LCD_COMMAND);
}

void LCDI2C::lcd_char(char val) 
{
    lcd_send_byte(val, LCD_CHARACTER);
}

void LCDI2C::lcd_string(const char *s) 
{
    while (*s) 
    {
        lcd_char(*s++);
    }
}

void LCDI2C::lcd_init() 
{
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}



int LCDI2C::getMaxRows()
{
  return maxLines;
}

int LCDI2C::getMaxColumns()
{
  return maxChars;
}





