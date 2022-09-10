#ifndef _I2C_H
#define _I2C_H
//外部文件引用

//宏定义区
#define SDA 0
#define SCL 1
#define I2C P3OUT

//函数声明
uint8_t I2C_Read_Byte(uint8_t Slaveaddr, uint8_t REG_Address);
void I2C_Write_Byte(uint8_t Slaveaddr, uint8_t REG_Address, uint8_t REG_data);
void I2C_Init();


#endif


/******************* (C) 版权所有 2018 北京中科浩电科技有限公司 *******************/
