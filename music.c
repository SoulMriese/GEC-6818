#include "touch.h"
#include "lcd.h"
#include "jpg.h"
#include <stdlib.h>
#include <stdio.h>	

struct LcdDev * lcd;
int x,y;
int ts_fd;
int music(int argc, char const *argv[])
{
	int i=0;
    int flag1 = 0;
    int flag2 = 2;
    char music[20] = {0};
    lcd = lcd_init(LCD_PATH);
	ts_fd = Touch_Init(TOUCH_SCREEN_PATH);
	bmp_show(lcd,"./pic/musicplay2.bmp");
	while(1)
	{
		//printf("in\n");
		if(x>410&&x<580&&y>290&&y<460)
		{
		    if(flag1 == 0)
		    {
		    	printf("Play Music\n");
		    	bmp_show(lcd,"./pic/musicpause1.bmp");
		        system("madplay ./music/1.mp3 &");
		        flag1 = 1;
		        x=0;
		        y=0;
	        }
	        else if(flag2 == 1)
	        {
	        	printf("Countinue Play\n");
	        	bmp_show(lcd,"./pic/musicpause1.bmp");
	    	    system("killall -CONT madplay");
			    flag2 = 0;
			    x=0;
		        y=0;
	        }
	        else if(flag1==1 || flag2==0)
	        {
	        	printf("Pause Play\n");
	        	bmp_show(lcd,"./pic/musicplay2.bmp");
	    	    system("killall -STOP madplay");
			    flag1 = 2;
			    flag2 = 1;
			    x=0;
		        y=0;
	        }  
	    }
	    if(x>400&&x<600&&y>500&&y<580)
	        {
	        	system("killall -KILL madplay");
	        	x=0;
	        	y=0;
	        	menu();
	        }
	}
}