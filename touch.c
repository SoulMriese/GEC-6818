#include "touch.h"



int Touch_Init(const char *touch_path)
{

	//1.打开触摸屏
	int ts_fd = open(touch_path,O_RDWR);
	if(ts_fd < 0)
	{
		perror("Open Touch Screen Error");
		return -1;
	}
	
	return ts_fd;

}

int get_xy(int* ts_x,int* ts_y,int ts_fd)
{

	//2.定义一个结构体
	struct input_event ts_event;
	while(1)
	{
		//3.获取数据
		read(ts_fd,&ts_event,sizeof(ts_event));
	
		//4.分析数据
		if(ts_event.type == EV_ABS)//触摸屏
		{
			if(ts_event.code == ABS_X)//横坐标
			{
				*ts_x = ts_event.value;
				
			}
			if(ts_event.code == ABS_Y)//纵坐标
			{

				*ts_y = ts_event.value;
			}	
		}	
		if(ts_event.type == EV_KEY && ts_event.code ==BTN_TOUCH &&ts_event.value == 0)//判断手指是否松开
		{

			printf("x = %d,y = %d\n",*ts_x,*ts_y);
			break;
		}		
	}
}