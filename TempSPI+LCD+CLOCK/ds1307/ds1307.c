#include "ds1307.h"

//          regaddr01,seconds,minutes,hours,weekdays,days,month,year
char  buf[]={0x00,  0x10,  0x30,    0x21, 0x03,    0x06,0x04, 0x21};
                  
char  *week[]  ={"SUN","Mon","Tues","Wed","Thur","Fri","Sat"};
int ds1307SetTime()
{
	return	i2c_write_blocking(i2c0,addr01,buf,8,false);
}

void ds1307ReadTime() 
{   
	uint8_t val = 0x00;  
	i2c_write_blocking(i2c0,addr01,&val,1,true);
	i2c_read_blocking(i2c0,addr01,buf,7,false);
} 

//#define I2C_PORT i2c0
