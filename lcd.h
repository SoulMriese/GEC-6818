#ifndef  _LCD_H
#define  _LCD_H


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
#include <linux/videodev2.h>
#include "jpeglib.h"
#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>  
#include <strings.h>
#include <pthread.h>


#define LCD_PATH	"/dev/fb0"


struct  LcdDev
{
	int fd; //保存LCD屏的文件描述符
	unsigned int * mp;//保存内存映射的首地址
};

//LCD初始化函数
struct LcdDev * lcd_init(const char *lcdname);


#endif