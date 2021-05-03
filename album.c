#include "touch.h"
#include "bmp.h"
#include "lcd.h"

int x,y;
int ts_fd;
struct LcdDev * lcd;
int album(int argc, char const *argv[])
{
	
	int i=0;
	lcd = lcd_init(LCD_PATH);
	ts_fd = Touch_Init(TOUCH_SCREEN_PATH);
	bmp_show(lcd,"./pic/album.bmp");
	char buf[20] = {0};
	while(1)
	{
		if(x>0&&x<100&&y>0&&y<100) //左上角  切换上一张
		{
			i++;
			if(i>5)
			{
				i=1;
			}
			sprintf(buf,"./pic/%d.bmp",i);
			bmp_show(lcd,buf);
			x=0;
			y=0;
		}
		else if(x>900&&x<1024&&y>0&&y<100) //右上角  切换下一张
		{
			i++;
			if(i>5)
			{
				i=1;
			}
			sprintf(buf,"./pic/%d.bmp",i);
			bmp_show(lcd,buf);
			x=0;
			y=0;
		}
		if(x>375&&x<630&&y>440&&y<510)//退出到登录按钮
    	{
    		menu();
    		x=0;
    		y=0;
    	}
	}


	return 0;
}