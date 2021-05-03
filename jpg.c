#include "jpg.h"



int show_jpg(struct LcdDev *lcd,int start_x,int start_y,const char *jpgpath)
{
	//1.定义解码对象、错误处理对象
	struct jpeg_decompress_struct info;//解码对象
	struct jpeg_error_mgr err;//错误处理对象

	//2.绑定错误处理
	info.err =  jpeg_std_error(&err);//需要取地址

	//3.打开解码文件（图片）
	FILE *file = fopen(jpgpath,"rb");
	if(file == NULL)
	{
		perror("fopen jpg fail\n");
		return -1;
	}

	//4.初始化解码对象
	jpeg_create_decompress(&info);

	//5.关联数据
	jpeg_stdio_src(&info,file);

	//6.读取jpeg文件头信息
	jpeg_read_header(&info,TRUE);

	//设置参数
	
	//7.开始解码
	jpeg_start_decompress(&info);

	//8.分配空间，存储解码好的数据
	unsigned int rowsize = info.output_width * info.output_components;//一行大小 = 宽*色深
	unsigned char *mp = malloc(rowsize);//申请堆空间存放数据

	//9.循环读取数据，读完一张图片为止
	while(info.output_scanline < info.output_height)
	{
		//读取的过程 rgb rgb rgb  
		jpeg_read_scanlines(&info,&mp,1);//解码对象  存放数据的地址  读取的最大行数
		int i=0;
		unsigned int data;//存放3字节转4字节的数据
		unsigned char *tp = mp;//防止指针丢失

		for(i=0;i<info.output_width;i++)
		{
			//把读取到的3字节数据，存入4字节空间
			data |= tp[0]<<16;
			data |= tp[1]<<8; 
			data |= tp[2];

			tp += 3;
			//scanline已经读过1行，所以不是从0开始
			*(lcd->mp +start_x+start_y*800 +(info.output_scanline-1)*800+i ) = data;
			data= 0;
		}
	}
	//10.解码完成
	jpeg_finish_decompress(&info);

	//11.释放内存，关闭文件
	jpeg_destroy_decompress(&info);
	fclose(file);

}