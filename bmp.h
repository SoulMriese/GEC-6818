#ifndef  BMP_H
#define  BMP_H


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <unistd.h>
#include "lcd.h"


/*************************************************

	函数作用：显示BMP图片

	函数参数：
			 lcd_fd   LCD屏的文件描述符
			 lcd_mp   LCD屏内存映射首地址
			 bmp_name BMP图片的名字

	返回值：成功  返回 0
		   失败  返回-1
************************************************/
int  bmp_show(struct  LcdDev *lcd,const char * bmp_name);




#endif