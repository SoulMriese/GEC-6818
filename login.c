#include "touch.h"
#include "lcd.h"
#include "jpg.h"
#include "camera.h"
#include "bmp.h"

int x,y; 	//保存触摸屏坐标
int ts_fd;	//保存触摸屏文件描述符
struct LcdDev * lcd;

int login(int argc, char const *argv[])
{
	lcd = lcd_init(LCD_PATH);
	ts_fd = Touch_Init(TOUCH_SCREEN_PATH);
    bmp_show(lcd,"./pic/login.bmp");
	while(1)
	{
		if(x>80&&x<380&&y>445&&y<520){
			menu();
			x=0;
			y=0;
		}
		else if(x>629&&x<920&&y>443&&520){
			printf("886\n");
			break;
		}
	}
	return 0;
}