#include "lcd_i2c.h"
#include "bme280.hpp"

extern "C"{
#include "ds3231.h" //some basic libraries are defined here inside
#include "ws2812.h"
}

#include <string>
#include <math.h>
//#include <iostream>
//#include <stdio.h>
//include <stdlib.h>
//#include "pico/stdlib.h"

#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"


using namespace pimoroni;
BME280 bme280;
int transistor = 22;   //c1815-npn
const int PIN_TX = 16; //Neopixel-LED
LCDI2C lcd(0x27 , 2, 16, 12, 13, 0);
//constructor-> LCDI2C(address,rows,columns,SDA,SCL,i2cPort);

std::string temperatura;
std::string time;
std::string CO2 = "0.0";
std::string color;


void FirstLEDDisplay(const std::string& text1);
void SecondLEDDisplay();
void ClockRoutine();
void TempRoutine();
void init_TempSensor();
void LedRoutine();
void TransistotRoutine();



int main() 
{  
   stdio_init_all();
   gpio_init(PICO_DEFAULT_LED_PIN);
   gpio_init(transistor);
   gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
   gpio_set_dir(transistor, GPIO_OUT);
   
   //sleep_ms(1000); 
   printf("WhaterStation Test Program ...\n\n");
   init_TempSensor();
   lcd.init();   
   
   PIO pio = pio0;
   int sm = 0;
   uint offset = pio_add_program(pio, &ws2812_program);
   char str[12];
   ws2812_program_init(pio, sm, offset, PIN_TX, 800000, true);  
   sleep_ms(100);  
   
    //ds3231SetTime(); //UNCOMMENT to set the time!!!  set time in ds3231.c
    while(1)  
    {  
       ClockRoutine();
       TempRoutine();
       LedRoutine();
       FirstLEDDisplay(temperatura);
       TransistotRoutine();
    } 
   return 0;
}


void FirstLEDDisplay(const std::string& text1)
{	
	lcd.lcd_set_cursor(0, 2); // line = 0, column = 2;
	lcd.lcd_string("Hola Hemosa!");

	lcd.lcd_set_cursor(1, 0); // line = 1, column = 0;
	lcd.lcd_string("Smile Beautiful");	
  sleep_ms(3000); 

  lcd.lcd_clear();

  lcd.lcd_set_cursor(0, 0); // line = 0, column = 0;
  std::string temptext = "Temp: ";
  temptext+= temperatura;
	lcd.lcd_string(temptext.c_str());
  sleep_ms(500);
  lcd.lcd_set_cursor(1, 0); // line = 0, column = 0;
  std::string co2text = "CO2: ";
  co2text+= CO2;
	lcd.lcd_string(co2text.c_str());

  sleep_ms(2000); 
}

void ClockRoutine()
{
  ds3231ReadTime();
	buf[0] = buf[0]&0x7F; //sec ok
	buf[1] = buf[1]&0x7F; //min ok
	buf[2] = buf[2]&0x3F; //hour ok
	buf[3] = buf[3]&0x07; //week
	buf[4] = buf[4]&0x3F; //day
	buf[5] = buf[5]&0x1F; //mouth
	//year/month/day
	printf("20%02x/%02x/%02x  ",buf[6],buf[5],buf[4]);
	//hour:minute/second
	printf("%02x:%02x:%02x  ",buf[2],buf[1],buf[0]);
	//weekday
	printf("%s\n",week[(unsigned char)buf[3]-1]);
	//printf("%s\n","laber nicht!");

  //int a_size = sizeof(buf) / sizeof(char);
  //size_t size = sizeof(buf) / sizeof(buf[0]);
  int size = strlen(buf);
  
  char hora[10];
  char fecha[12];
  char timestamp[10]; 
  sprintf(hora, "%02x:%02x",buf[2],buf[1]);
  sprintf(fecha, "20%02x/%02x/%02x",buf[6],buf[5],buf[4]);
  
  printf("%s\n", hora);
  printf("%s\n", fecha);
  
  lcd.lcd_clear();
  lcd.lcd_set_cursor(0, 0); // line = 1, column = 0;
  lcd.lcd_string("     ");
	lcd.lcd_string(hora);	
  sleep_ms(500); 
  lcd.lcd_set_cursor(1, 0); // line = 1, column = 0;
  lcd.lcd_string("   ");
	lcd.lcd_string(fecha);
  sleep_ms(1000); 	

  sprintf(timestamp, "20%02x%02x%02x%02x%02x",buf[6],buf[5],buf[4],buf[2],buf[1]);
  time = std::string(timestamp);
  printf("%s\n", time.c_str());
}


void TempRoutine()
{
    bme280.update_sensor();

    float temperature = bme280.get_temperature()-1.0; //-1.0 to Calibrate, check yours!
    float pressure = bme280.get_pressure();
    float humidity = bme280.get_humidity();

    printf("Temperature: %f\n", temperature);
    //printf("Pressure: %f\n", pressure);
    //printf("Humidity: %f\n", humidity);
    
    std::string first_four = std::to_string(temperature).substr(0, 4);
    temperatura = first_four;//std::to_string(temperature); 
}

void init_TempSensor()
{
if(!bme280.init()) //default SDA, SCL -> 4, 5 GPIO
{
    while(true) {
    printf("BME280 Failed to init!\n");
    sleep_ms(200);
    //return 1;
    }
  }
}

void LedRoutine()
{
    puts("Hello RGB and other colors!");
    // OPTION A: uncomment sleep_ms(500) and view the individual colors
    // OPTION B: comment sleep_ms(500) and view the pixels cascading colors
    /*put_pixel(urgb_u32(0xff, 0, 0));  // Red
    sleep_ms(500);
    put_pixel(urgb_u32(0, 0xff, 0));  // Green
    sleep_ms(500);
    put_pixel(urgb_u32(0, 0, 0xff));  // Blue
    sleep_ms(500);
    put_pixel(urgb_u32(0xff, 0xff, 0));  // Purple
    sleep_ms(500);
    put_pixel(urgb_u32(0, 0xff, 0xff));  // Cyan
    sleep_ms(500);
    put_pixel(urgb_u32(0xff, 0xff, 0xff));  // White
    sleep_ms(500);
    put_pixel(urgb_u32(0, 0, 0));  // Black or off
    sleep_ms(500);*/
    // Clear all pixels
    printf("Temperature---> %s\n", temperatura.c_str());
    printf("CO2---> %s\n", CO2.c_str());

    float temp_float = std::stof(temperatura);
    float co2_float = std::stof(CO2);
    if(temp_float > 23.0 | co2_float > 1000.0)
    {
      put_pixel(urgb_u32(0xff, 0, 0));  // Red
      sleep_ms(500);
    }
    else if (temp_float < 20.0 )
    {
      put_pixel(urgb_u32(0, 0, 0xff));  // Blue
      sleep_ms(500);
    }
    sleep_ms(1000);
    for (int i = 0; i <= 60; i++) {
      put_pixel(urgb_u32(0, 0, 0));  // Black or off
    }
    sleep_ms(100);
}

void TransistotRoutine()
{
    gpio_put(PICO_DEFAULT_LED_PIN, true);
    gpio_put(transistor, true);
    sleep_ms(500);
    gpio_put(PICO_DEFAULT_LED_PIN, false);
    gpio_put(transistor, false);
    sleep_ms(500); 
}



