#include <xc.h>
#include "ds1307.h"
#include "i2c.h"
#include "rtc.h"
#include "clcd.h"

unsigned char clock_reg[3];
extern unsigned char time[9];

void display_time(void)
{
	clcd_print(time, LINE2(0));
}

 void get_time(void)
{
    // Initially storing the hr,min,sec
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

    //Checking whether time is 12/24hr format
	if (clock_reg[0] & 0x40)
	{
        //IF IT IS 12HR FORMAT WE NEED TO EXTRACT ONLY BIT4 AND BIT0 -BIT3
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
        //IF IT IS 24HR FORMAT WE NEED TO EXTRACT ONLY BIT4-BIT5 AND BIT0 -BIT3
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
    
}
