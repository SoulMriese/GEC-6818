#ifndef _TOUCH_H
#define _TOUCH_H

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <linux/input.h>

#define TOUCH_SCREEN_PATH  "/dev/input/event0"


int Touch_Init(const char *touch_path);
int get_xy(int* ts_x,int* ts_y,int ts_fd);

#endif