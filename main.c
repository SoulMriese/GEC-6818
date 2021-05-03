#include "lcd.h"
#include "touch.h"
#include "jpg.h"
#include "camera.h"

/*
1901532117 杨锦鹏  Main函数代码
 */
int x,y;
int ts_fd;
struct LcdDev * lcd;  

char  buf[20] = {0};
//不断获取触摸屏坐标
void * thread1_fun(void * arg)
{	
	while(1)
	{
		get_xy(&x,&y,ts_fd);
		x = x / 1024 * 800;
		y = y / 600  * 480;
	}
}

//不断获取摄像头图像
void * thread2_fun(void * arg)
{	
	while(1)
	{
		camera_get_image();
	}
}




int main()
{
	int i=0;//开机动画变量i
	//1.打开LCD
	lcd = lcd_init(LCD_PATH);
	printf("Open Lcd Device Normal\n");
	
	//2.打开触摸屏
	ts_fd = Touch_Init(TOUCH_SCREEN_PATH);
	printf("Open Touch Screen Normal\n");
	

	//4.创建两条线程
	pthread_t  thread1,thread2;
	pthread_create(&thread1,NULL,thread1_fun,NULL);
	pthread_create(&thread2,NULL,thread2_fun,NULL);
    for(i=0;i<=144;i++)//1.开机动画
	{
		sprintf(buf,"./gif/Frame%d.jpg",i);//按顺序打印所有jpg图片，组成开机动画
		show_jpg(lcd,0,0,buf);//调用JPG库
		usleep(500);//图片显示速度为0.5秒
	}
	//5.显示登陆界面
	login();

}