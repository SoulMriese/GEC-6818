#include "lcd.h" // ""代表先从当前路径找，在去系统路径找   < >   只去系统路径找


struct LcdDev * lcd_init(const char *lcdname)
{
	struct LcdDev * lcd = malloc(sizeof(struct LcdDev));//申请堆空间

	//1.打开LCD   int fd 
	lcd->fd = open(lcdname,O_RDWR);
	if( lcd->fd == -1)
	{
		perror("Open Lcd Error");
	}
	
	//2.内存映射  unsigned int *
	lcd->mp = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE, MAP_SHARED,lcd->fd,0);
	if(lcd->mp == NULL)
	{
		perror("mmap error");
	}
	
	
	return lcd;
	
}

