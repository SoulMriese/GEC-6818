#include "bmp.h"


int  bmp_show(struct  LcdDev *lcd,const char * bmp_name)
{
	//1.打开BMP  
	int bmp_fd = open(bmp_name,O_RDONLY);
	if(bmp_fd == -1)
	{
		perror("open bmp error");
		return -1;
	}

	//2.跳过BMP图片的信息  lseek  54
	lseek(bmp_fd,54,SEEK_SET);

	//3.读取BMP图片的颜色数据  存放在一个数组
	char bmp_buf[800*480*3] = {0};

	int cnt = read(bmp_fd,bmp_buf,800*480*3);
	if(cnt != 800*480*3)
	{
		perror("read bmp data error");
		return -1;
	}

	//4.关闭BMP  
	close(bmp_fd);

	//5.对读取的颜色数据进行处理  把3字节数据放入4字节的空间
	int i,j;
	int lcd_buf[800*480] = {0};//lcd_buf相当于一块新的屏幕

	for(i=0,j=0;i<800*480;i++,j+=3)
	{
		lcd_buf[i] = ( bmp_buf[j] ) | ( bmp_buf[j+1]<<8 ) | ( bmp_buf[j+2]<<16 );
	}


	//6.实现图像翻转

	int x,y;

	for(y=0;y<480;y++)
	{
		for(x=0;x<800;x++)
		{
			*(lcd->mp+y*800+x) = lcd_buf[(479-y)*800+x];
		}
	}
}