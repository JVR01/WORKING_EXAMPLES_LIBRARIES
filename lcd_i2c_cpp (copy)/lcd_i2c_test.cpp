#include "lcd_i2c.h"

void FirstLEDDisplay();
void SecondLEDDisplay();

int main() 
{  
   FirstLEDDisplay();
   //SecondLEDDisplay();
   
	
    return 0;
}


void FirstLEDDisplay()
{
	LCDI2C lcd(0x27 , 2, 16, 12, 13, 0);//LCDI2C lcd(0x27 , 2, 16, 16, 17, 0);
        //constructor-> LCDI2C(int addr, int rows, int columns, unsigned int pinSDA, unsigned int pinSCL, int i2cPort);
	lcd.init();
	
	lcd.lcd_set_cursor(0, 0); // line = 0, column = 0;
	lcd.lcd_string("WaterMelon");

	lcd.lcd_set_cursor(1, 0); // line = 1, column = 0;
	lcd.lcd_string("Orange");

	//lcd.lcd_set_cursor(2, 0); // line = 2, column = 0;
	//lcd.lcd_string("Apple");

	//lcd.lcd_set_cursor(3, 0); // line = 3, column = 0;
	//lcd.lcd_string("Banana");	
	
}

void SecondLEDDisplay()
{	  
    LCDI2C lcd2(0x27, 2, 16, 14, 15, 1);    
    lcd2.init();	
	
    lcd2.lcd_set_cursor(0, 0); // line = 0, column = 0;
    lcd2.lcd_string("Sparrow");

    lcd2.lcd_set_cursor(1, 0); // line = 1, column = 0;
    lcd2.lcd_string("Eagle");
}



