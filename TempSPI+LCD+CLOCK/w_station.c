/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "ds1307.h"

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"



#define MISO 16
#define CS   17
#define SCLK 18
#define MOSI 19

#define SPI_PORT spi0

int32_t t_fine;
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
/* Example code to drive a 16x2 LCD panel via a I2C bridge chip (e.g. PCF8574)

   NOTE: The panel must be capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefor I2C) cannot be used at 5v.

   You will need to use a level shifter on the I2C lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO 4 (pin 6)-> SDA on LCD bridge board
   GPIO 5 (pin 7)-> SCL on LCD bridge board
   3.3v (pin 36) -> VCC on LCD bridge board
   GND (pin 38)  -> GND on LCD bridge board
*/
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

#define I2C_PORT i2c0
// By default these LCD display drivers are on bus address 0x27
static int addr = 0x27;

// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES      2
#define MAX_CHARS      16
//################## Start temperatur Stuff
int32_t compTemp(int32_t adc_T){
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1))) * ((int32_t) dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) dig_T1)) * ((adc_T >> 4) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3)) >> 14;

    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

void read_temp_comp(){
    uint8_t buffer[6], reg;

    reg = 0x88 | 0x80;
    gpio_put(CS, 0);
    spi_write_blocking(SPI_PORT, &reg, 1);
    spi_read_blocking(SPI_PORT, 0, buffer, 6);
    gpio_put(CS, 1);

    dig_T1 = buffer[0] | (buffer[1] << 8);
    dig_T2 = buffer[2] | (buffer[3] << 8);
    dig_T3 = buffer[4] | (buffer[5] << 8);
}


///###############end temperatur Stuff
/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val) {
    i2c_write_blocking(I2C_PORT, addr, &val, 1, false);
}

void lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
#define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

// go to location on LCD
void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

static void inline lcd_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

void lcd_string(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

void lcd_init() {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}

int main() {

    stdio_init_all(); // Initialise I/O for USB Serial

    spi_init(SPI_PORT, 500000); // Initialise spi0 at 500kHz
    
    //Initialise GPIO pins for SPI communication
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(SCLK, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);

    // Configure Chip Select
    gpio_init(CS); // Initialise CS Pin
    gpio_set_dir(CS, GPIO_OUT); // Set CS as output
    gpio_put(CS, 1); // Set CS High to indicate no currect SPI communication
    read_temp_comp(); // Read factory calibration/compensation values


    // Write Operation Example! Set oversampling and power on chip
    uint8_t data[2]; // Array to store data to be sent
    data[0] = 0xF4 & 0x7F; // Remove first bit to indicate write operation
    data[1] = 0x27; // Data to be sent
    gpio_put(CS, 0); // Indicate beginning of communication
    spi_write_blocking(SPI_PORT, data, 2); // Send data[]
    gpio_put(CS, 1); // Signal end of communication



    int32_t temperature, rawtemp;
    uint8_t reg, buffer[3]; 
    // This example will use I2C0 on GPIO4 #20 (SDA) and GPIO5 #21 (SCL)
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(12, GPIO_FUNC_I2C); //20
    gpio_set_function(13, GPIO_FUNC_I2C); //21
    gpio_pull_up(12); //20
    gpio_pull_up(13); //21
    // Make the I2C pins available to picotool
    bi_decl( bi_2pins_with_func(12, 13, GPIO_FUNC_I2C)); //4,5


    //CLOCK
    /*gpio_set_function(12,GPIO_FUNC_I2C);//6
    gpio_set_function(13,GPIO_FUNC_I2C);//7
    */
	//gpio_pull_up(12); //6
    //gpio_pull_up(13); //7
    printf("DS1307 Test Program ...\n\n"); 



    lcd_init();
    //some cast sh..t I dont realy undertand yet, has to do with convert a pointer avriable to something else 
    

    while (1) {
        
        reg = 0xFA | 0X80;
        gpio_put(CS, 0);
        spi_write_blocking(SPI_PORT, &reg, 1);
        spi_read_blocking(SPI_PORT, 0, buffer, 3);
        gpio_put(CS, 1);

        rawtemp = ((uint32_t) buffer[0] << 12) | ((uint32_t) buffer[1] << 4) | ((uint32_t) buffer[2] >> 4);

        temperature = compTemp(rawtemp);
        //int32_t temperature;
        printf("Temp = %.2fC \n", temperature / 100.00);

        //sleep_ms(1000);
        float TempFloat =  temperature / 100.00;
        char Temperatura[12];
        sprintf(Temperatura, "%f", TempFloat);

        //float realTemp = 0.0775*TempFloat*TempFloat - 2.5151*TempFloat + 38.019;
        float realTemp = 0.0494*TempFloat*TempFloat - 1.3214*TempFloat + 25.375;
        char realTemperatura[12];


        
        ds1307ReadTime();
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
		//sleep_ms(1000); 



        //char message02[] = ("%02x:%02x:%02x  ",buf[2],buf[1],buf[0]);
        //char name[10] = {buf[2] , ':' , buf[1] ,':',buf[0],'\0'}; 
        char hora[10];
        char fecha[12];
        //sprintf(hora, "%02x", name);
        //++sprintf(hora, "%02x:%02x:%02x",buf[2],buf[1],buf[0]);
        sprintf(hora, "%02x:%02x",buf[2],buf[1]);
        sprintf(fecha, "20%02x/%02x/%02x",buf[6],buf[5],buf[4]);

        //printf("%02x\n", hora);
        printf("\n");
        printf(fecha);
        printf("\n");
        printf(hora);
        printf("\n");
        //xxxprintf("%c-%c\n",hora, fecha);
        /*printf("%02x\n", buf[6]);
        printf("%s\n", name);
        printf("%s\n", hora);
        printf("%s\n", buf[6]);
        printf("%d\n", buf[6]);*/

        /*char str1[100] = "This is ";
        char str2[] = "pRogramiz.com";
        strcat(str1, str2);
        printf(str1);*/
        
        //= ("%02x:%02x:%02x  ",buf[2],buf[1],buf[0]);


        sprintf(realTemperatura, "%0.1f", realTemp); //%f Orig
        /*static*/ uint8_t *message[] =
            {
                    "Hola Hemosa", "Smile beautiful!",
                    //"A brand new", "microcontroller",
                    "Temp [C]:", realTemperatura,
                    //"More power in", "your product",
                    //"Temp: ", Temperatura,
                    "SmileTime :)", hora,
                    "Be happy day: ", fecha
            };


        for (int m = 0; m < sizeof(message) / sizeof(message[0]); m += MAX_LINES) {
            for (int line = 0; line < MAX_LINES; line++) {
                lcd_set_cursor(line, (MAX_CHARS / 2) - strlen(message[m + line]) / 2);
                lcd_string(message[m + line]);
            }
            sleep_ms(5000);
            lcd_clear();
        }
    }

    return 0;
}
