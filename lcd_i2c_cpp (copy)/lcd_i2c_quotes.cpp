#include <time.h>
#include <stdlib.h>

#include "lcd_i2c.h"

/* Example for a 16x2 LCD panel via a I2C bridge chip
  
   GPIO 4 (pin 6)-> SDA on LCD bridge board
   GPIO 5 (pin 7)-> SCL on LCD bridge board
   +5V (power bank) -> VCC on LCD bridge board
   GND (pin 3 or 38)  -> GND on LCD bridge board
*/

void displayQuote(const char * quote, LCDI2C lcd);
char * SubString(const char * source, int startindex, int count, char * dest);

int main() 
{
    char const *message[] =
    {
	    "1. The secret of getting ahead is getting started.",
	    "2. Only surround yourself with people who will lift you higher.",
	    "3. No one is perfect – that’s why pencils have erasers.",                    
	    "4. Fall seven times, stand up eight.",
	    "5. You always pass failure on the way to success.",
	    "6. The only man who never makes a mistake is the man who never does anything."                    
	    "7. Do what you can, with what you have, where you are.",
	    "8. Have a heart that never hardens, and a temper that never tires and a touch that never hurts.",
	    "9. We grow great by dreams.",
	    "10. It is not what happens to you, but how you react to it that matters.",
	    "11. No one can make you feel inferior without your consent."
    };
    
    int arraySize = sizeof(message)/sizeof(message[0]);
    
    srand(time(NULL)); // Initialization random number
    int ranNum, index;      
    
    //LCDI2C lcd; // Default addr=0x27, rows=2, columns=16, pinSDA=4, pinSCL=5, i2cPort = 0 ;   
   
    LCDI2C lcd(0x27 , 2, 16, 12, 13, 0);//LCDI2C lcd(0x27, 4, 20, 16, 17, 0);
    lcd.init();  

    while (1)
    {    
    	ranNum = rand(); // Get random number (pseudo-random integer between 0 and RAND_MAX)
    	index = ranNum % arraySize; // Get random number between 0 to arraySize-1
    	
    	displayQuote(message[index], lcd);
    	lcd.lcd_clear();
    	sleep_ms(200); //sleep_ms(1200);
    }

    return 0;
}

void displayQuote(const char * quote, LCDI2C lcd)
{
    int length = strlen(quote);
    char msg[21]; // Maximum column size for 16x2 or 20x4 LCD display
    int i = 0;    
    int lcdColumns = lcd.getMaxColumns();

    while ((i + lcdColumns) <= length)
    {
        SubString(quote, i, lcdColumns, msg);

	lcd.lcd_set_cursor(0, 0); // line = 0, column = 0;
	lcd.lcd_string(msg);
	
	sleep_ms(400);//sleep_ms(800);
        lcd.lcd_clear();

        i++;
    }
}

char * SubString(const char * source, int startindex, int count, char * dest)
{
   int i = 0;

   while (i < count && source[startindex] != '\0')
   {
       dest[i] = source[startindex];
       i++;
       startindex++;
   }
   dest[i] = '\0';

   return dest;
}


