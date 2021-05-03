#include "touch.h"
#include "lcd.h"
#include "jpg.h"
#include "camera.h"
#include "bmp.h"


int x,y; 	//保存触摸屏坐标
int ts_fd;	//保存触摸屏文件描述符 

struct LcdDev * lcd;

int menu(int argc, char const *argv[])
{
	lcd = lcd_init(LCD_PATH);
	ts_fd = Touch_Init(TOUCH_SCREEN_PATH);
		test:

			bmp_show(lcd,"./pic/menu2.bmp");

			while(1)
			{
				if(x>375&&x<630&&y>510&&y<600)//退出
    	        {
    		      login();
    		      x=0;
    		      y=0;
    	        }

    	        if(x>100&&x<300&&y>290&&y<460)//相册
    	        {
    		      album();
    		      x=0;
    		      y=0;
    	        }

				if(x>400&&x<600&&y>290&&y<460)
				{
					camerashow();
				}
			    if(x>700&&x<900&&y>290&&y<460)	
				{
					music();
				}
		}
	return 0;
}
