#ifndef _JPG_H
#define _JPG_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>          
#include <fcntl.h>            
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>        
#include "jpeglib.h"    //该头文件中的函数就是jpeg解码需要用到的
#include "lcd.h"

/*************************************************

	函数作用：显示JPG图片

	函数参数：
			 lcd   		 LCD屏的设备结构体的指针
			 start_x    图像显示的起始X轴坐标
			 start_y    图像显示的起始Y轴坐标
			 jpgpath    JPG图片的名字

	返回值：成功  返回 0
		   失败  返回-1
************************************************/
int show_jpg(struct LcdDev *lcd,int start_x,int start_y,const char *jpgpath);



#endif