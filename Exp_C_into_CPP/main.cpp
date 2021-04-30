
#include <stdio.h>
#include "pico/stdlib.h"

extern "C"{
#include "ds3231.h"
}


int main()  
{
    stdio_init_all();
    i2c_init(i2c0,100*1000);
    gpio_set_function(12,GPIO_FUNC_I2C);//6
    gpio_set_function(13,GPIO_FUNC_I2C);//7
    gpio_pull_up(12); //6
    gpio_pull_up(13); //7
	
		
    printf("DS1307 Test Program ...\n\n"); 
   	
    
    //ds3231SetTime(); //UNCOMMENT to set the time!!!  set time in ds3231.c
    while(1)  
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
		sleep_ms(1000); 
    }  
	return 0;
}
