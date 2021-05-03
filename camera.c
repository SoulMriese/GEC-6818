#include "lcd.h"
#include "camera.h"
#include "jpeglib.h"

extern volatile int x,y;
extern volatile struct LcdDev * lcd;  

int flag_show = 0;
int flag_pic = 0;

struct buffer * buffers;//保存缓存块信息
int camera_fd = 0;//保存摄像头的文件描述符





unsigned int  * rgb32buf;//为了存储转换后的一帧数据  得到的每一个像素点都是ARGB
unsigned char * rgb24buf;//为了存储转换后的一帧数据  得到的每一个像素点都是RGB

//转换一个像素点
int yuyv_to_rgb32_pix(int y,int u,int v)
{
	unsigned int pix_point = 0;//存储一个像素点的数据

	int r,g,b;

	//通过V4L2官方帮助文档中的公式进行计算
	r  = 1.164*(y-16) + 1.589*(v-128);
	g  = 1.164*(y-16) - 0.380*(u-128) - 0.810*(v-128);
	b  = 1.164*(y-16) + 2.010*(u-128);

	//处理越界  RGB分别为占用一个字节 0~255，所以需要进行越界处理
	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;

	pix_point = (0x00<<24)|(r<<16)|(g<<8)|b;//把RGB合成一个整型   LCD屏 RGB---ARGB
	return pix_point;
}



//转换一帧ARGB数据（YUYV转RGB32）
int yuyv_to_rgb32(char * yuyvbuf,unsigned int * rgbbuf)
{
	int i,j;
	for(i=0,j=0;i<WIDTH*HEIGHT;i+=2,j+=4)
	{
		rgbbuf[i]   = yuyv_to_rgb32_pix(yuyvbuf[j],yuyvbuf[j+1],yuyvbuf[j+3]);   // argb  ----- y1u1y2v1  YUYV 
		rgbbuf[i+1] = yuyv_to_rgb32_pix(yuyvbuf[j+2],yuyvbuf[j+1],yuyvbuf[j+3]); 
	}  
}



//把ARGB转为RGB（RGB32转RGB24）
int rgb32_to_rgb24(unsigned int * rgb32buf,unsigned char *rgb24buf)
{
	int i;

	for(i=0;i<WIDTH*HEIGHT;i++)    // 320*240*4  ----  320*240*3
	{

		rgb24buf[i*3]   = rgb32buf[i]>>16;// ARGB  ----  R G B
		rgb24buf[i*3+1] = rgb32buf[i]>>8;
		rgb24buf[i*3+2] = rgb32buf[i];
	}

}



//把转换后的一帧ARGB数据写到LCD
int  camera_to_lcd(unsigned int *lcd_mp,unsigned int *rgbbuf,int start_x,int start_y)
{
	int x,y;

	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			*(lcd_mp+(start_y*800)+start_x + (y*800) + x) = rgbbuf[y*WIDTH+x];
		}
	}
}


//转换一帧数据（RGB24转JPEG）  
int rgb24_to_jpeg(unsigned char * rgb24_buf,int image_width,int image_height,const char * jpg_name)
{
	int  i,j;
	//1.定义压缩对象和错误处理对象
	struct  jpeg_compress_struct  cinfo;//压缩对象的结构体
	struct  jpeg_error_mgr  err; //错误处理对象

	//2.把错误处理对象绑定在压缩对对象上
	cinfo.err = jpeg_std_error(&err);

	//3.初始化压缩对象
	jpeg_create_compress(&cinfo);

	//4.设置压缩对象格式
	cinfo.image_width  		= WIDTH;	//图像宽度
	cinfo.image_height 		= HEIGHT;	//图像高度
	cinfo.input_components	= 3;		//图像色深
	cinfo.in_color_space 	= JCS_RGB; 
	jpeg_set_defaults(&cinfo);			//设置相关参数

	//5.设置压缩对象的质量
	jpeg_set_quality(&cinfo, 80, TRUE);

	//6.打开压缩文件  fopen 
	FILE *file =fopen(jpg_name,"w");//只写形式打开
	if(file == NULL)
	{
		perror("fopen jpg Error");
		return -1;
	}
	//绑定输出
	jpeg_stdio_dest(&cinfo, file);

	//7.开启压缩  无法再修改参数
	jpeg_start_compress (&cinfo, TRUE);

	//8. 申请内存空间去存储一行数据
	unsigned int  rowsize = cinfo.image_width * cinfo.input_components; //一行= 宽*色深
	unsigned char * row_pointer = calloc(rowsize,1);


	while( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer =  (rgb24_buf+cinfo.next_scanline*cinfo.image_width*cinfo.input_components);
		jpeg_write_scanlines (&cinfo, &row_pointer, 1);//每次写入一行
	}	
		
	//9.压缩完成  
	jpeg_finish_compress (&cinfo);

	//10.释放资源
	jpeg_destroy_compress (&cinfo);
	fclose(file);

	return 0;
}


	


//获取摄像头采集的图像
int camera_get_image()
{

	int i;
	int ret;

	//1.打开摄像头
	camera_fd = open(CAMERA_PATH,O_RDWR);
	if (camera_fd == -1)
	{
		perror("Open camera Error");
		return -1;
	}

	//2.设置视频捕获的格式
	struct v4l2_format fmt;
	memset(&fmt,0,sizeof(fmt));									//清空结构体
	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE; 		//数据流格式
    fmt.fmt.pix.width       = WIDTH; 							//帧宽
    fmt.fmt.pix.height      = HEIGHT;							//帧高
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;				//像素格式  摄像头只支持YUYV

    ret = ioctl(camera_fd,VIDIOC_S_FMT, &fmt);					//设置格式
    if (ret < 0)
    {
    	perror("VIDIOC_S_FMT Error");
    	return -1;
    }

    //3.申请缓存区
    struct v4l2_requestbuffers req;
    req.count               = 4;								//申请4个   一般4个左右
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;		//数据流格式
    req.memory              = V4L2_MEMORY_MMAP;					//内存映射方式
    ret = ioctl(camera_fd,VIDIOC_REQBUFS, &req);				//设置格式
    if (ret < 0)
    {
    	perror("VIDIOC_REQBUFS Error");
    	return -1;
    }

    //4.获取缓存块信息
    buffers = calloc (req.count, sizeof (struct buffer));

   	struct v4l2_buffer buf;

    for(i=0;i<req.count;i++)
    {
    	memset(&buf,0,sizeof(buf));	
    	buf.type      = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory    = V4L2_MEMORY_MMAP;
		buf.index     = i;										//索引  从0开始
		ret = ioctl (camera_fd, VIDIOC_QUERYBUF, &buf);			//查询缓存块信息
		if (ret < 0)
	    {
	    	perror("VIDIOC_QUERYBUF Error");
	    	return -1;
	    }

		buffers[i].length = buf.length;							//存放缓存大小
		buffers[i].start  = mmap(
									NULL ,
                              		buf.length,
                              		PROT_READ | PROT_WRITE,
                              		MAP_SHARED,
                              		camera_fd, 
                              		buf.m.offset
                              	);								//为缓存块申请堆空间

		//5.缓存块入队
		ret = ioctl (camera_fd, VIDIOC_QBUF, &buf);
		if (ret < 0)
	    {
	    	perror("VIDIOC_QBUF Error");
	    	return -1;
	    }
    }

	//6.开启视频采集
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl (camera_fd, VIDIOC_STREAMON, &type);
    if (ret < 0)
    {
    	perror("VIDIOC_STREAMON Error");
    	return -1;
    }


    //7.缓存块出队   获取数据
    fd_set fds;					//定义文件描述集
    struct timeval tv;			//设置超时时间  1s
 	memset(&tv,0,sizeof(tv));	

    tv.tv_sec = 1;
	tv.tv_usec = 0;

    rgb32buf = malloc( WIDTH*HEIGHT*4 );//为图像申请了一块堆空间
    rgb24buf = malloc( WIDTH*HEIGHT*3 );//为图像申请了一块堆空间
    
    while(1)
    {
    	
    	FD_ZERO (&fds);
    	FD_SET (camera_fd, &fds);

    	select (camera_fd + 1, &fds, NULL, NULL, &tv);//监听是否有数据  

    	//说明有数据
    	for(i=0;i<req.count;i++)
    	{
    		struct v4l2_buffer buf;
    		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;
            ret = ioctl(camera_fd, VIDIOC_DQBUF, &buf);//出队
			if (ret < 0)
			{
		    	perror("VIDIOC_DQBUF Error");
		    	return -1;
			}
			
            //处理数据  把YUYV格式转换为RGB
            yuyv_to_rgb32(buffers[i].start,rgb32buf);//得到了一帧ARGB数据
            rgb32_to_rgb24(rgb32buf,rgb24buf);//把一帧ARGB数据转换为RGB数据


            if(flag_show == 1)
            {
            	camera_to_lcd(lcd->mp,rgb32buf,240,120);//把得到的一帧ARGB数据显示在LCD上
            }
           	if(flag_pic == 1)
            {
            	rgb24_to_jpeg(rgb24buf,WIDTH,HEIGHT,JPG_PATH); //把RGB数据保存为一张jpg图片
            	sleep(2);//延时2秒  产生视觉暂留效果
            	flag_pic = 0;
            }


            ret = ioctl(camera_fd, VIDIOC_QBUF, &buf);//入队
			if (ret < 0)
			{
		    	perror("VIDIOC_QBUF Error");
		    	return -1;
			}
    	}
    }

    //8.停止采集
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl (camera_fd, VIDIOC_STREAMOFF, &type);
    if (ret < 0)
    {
    	perror("VIDIOC_STREAMOFF Error");
    	return -1;
    }
    //9.关闭文件，释放资源



}