#ifndef __MPU6050_H
#define __MPU6050_H

//外部文件引用
#include "stdint.h"
#include "I2C.h"

#define MPU6050_ADDRESS 0xD0


void MPU6050Init(void);

#endif 

