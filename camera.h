#ifndef  _CAMREA_H
#define  _CAMREA_H


#define   WIDTH         320  			//帧宽   也可以320  
#define   HEIGHT        240  			//帧高   也可以240
		
#define   LCD_SIZE		800*480*4  		//LCD屏的大小

#define   CAMERA_PATH	"/dev/video7"  //摄像头的设备节点   根据自己实际情况修改
#define   JPG_PATH		"camera.jpg"  //把采集到的一帧图像保存为jpg图片

//为了保存缓存块的信息
struct buffer {
        void *                  start; 	//缓存块的首地址
        unsigned int            length; //缓存块的大小
};





//转换一个像素点
int yuyv_to_rgb32_pix(int y,int u,int v);

//转换一帧ARGB数据（YUYV转RGB32）
int yuyv_to_rgb32(char * yuyvbuf,unsigned int * rgbbuf);

//把ARGB转为RGB（RGB32转RGB24）
int rgb32_to_rgb24(unsigned int * rgb32buf,unsigned char *rgb24buf);

//把转换后的一帧ARGB数据写到LCD
int camera_to_lcd(unsigned int *lcd_mp,unsigned int *rgbbuf,int start_x,int start_y);

//获取摄像头采集的图像
int camera_get_image(void);

#endif