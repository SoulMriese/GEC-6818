#include "touch.h"
#include "lcd.h"
#include "jpg.h"
#include "camera.h"
#include "bmp.h"

int x,y; 	//保存触摸屏坐标
int ts_fd;	//保存触摸屏文件描述符 
extern volatile int flag_show; //显示摄像头画面标志  flag_show = 1 显示画面  flag_show = 0 不显示画面
extern volatile int flag_pic ; 
struct LcdDev * lcd;

int camerashow(int argc, char const *argv[])
{
	lcd = lcd_init(LCD_PATH);
	ts_fd = Touch_Init(TOUCH_SCREEN_PATH);
	while(1){

	if(x>400&&x<600&&y>290&&y<460)//判断点击了监控
				{
					flag_show = 1;
					flag_pic  = 0;
				}
				if(x>375&&x<630&&y>510&&y<600) //判断点击了返回
				{
					flag_show= 0;
					flag_pic= 0;//不显示摄像头画面
					bmp_show(lcd,"./pic/menu2.bmp");
					x=0;
					y=0;
					menu();
				}
				else if(x>100&&x<300&&y>290&&y<460) 		//判断点击了拍照
				{
					flag_pic = 1;
					x=0;
					y=0;

				}	
				else if(x>700&&x<900&&y>290&&y<460) 	//判断点击了查看
				{
					flag_show = 0;
					show_jpg(lcd,240,120,JPG_PATH);
					x=0;
					y=0;
				}
}
}