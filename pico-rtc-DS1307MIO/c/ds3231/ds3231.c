#include "ds3231.h"

//          regaddr,seconds,minutes,hours,weekdays,days,month,year

char  buf[]={0x00,  0x10,  0x47,    0x18, 0x04,    0x28,0x04, 0x21};
// writes 2021/04/28  18:47:10  Wed
                  
char  *week[]  ={"SUN","Mon","Tues","Wed","Thur","Fri","Sat"};
int ds3231SetTime()
{
	return	i2c_write_blocking(i2c0,addr,buf,8,false);
}

void ds3231ReadTime() 
{   
	uint8_t val = 0x00;  
	i2c_write_blocking(i2c0,addr,&val,1,true);
	i2c_read_blocking(i2c0,addr,buf,7,false);
} 

//#define I2C_PORT i2c0
