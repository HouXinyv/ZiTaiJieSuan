//外部文件引用
#include "stdint.h"
#include "I2C.h"
#include "driverlib.h"

//私有函数区
void I2C_Start();
void I2C_Stop();
void I2C_RecvACK();
void I2C_SendACK(uint8_t ack);
void I2C_SendByte(uint32_t data);
uint32_t I2C_RecvByte();

//私有变量区
unsigned char Ack_Count = 0; 

/******************************************************************************
  * 函数名称：I2C_Init
  * 函数描述：初始化I2c总线
  * 输    入：void
  * 输    出：void
  * 返    回：void 
  * 备    注：null    
  *    
  *
******************************************************************************/
void I2C_Init()
{
  P3DIR |= (1 << SCL);//方向寄存器.p3.1设为输出
}

/******************************************************************************
  * 函数名称：delay
  * 函数描述：I2c延时
  * 输    入：
  * uint32_t n:延时的周期
  * 输    出：void
  * 返    回：void 
  * 备    注：null    
  *    
  *
******************************************************************************/
void delay(uint32_t n)
{
  while (n--);
}

/******************************************************************************
  * 函数名称：I2C_Start
  * 函数描述：I2c开始信号
  * 输    入：void
  * 输    出：void
  * 返    回：void 
  * 备    注：null    
  *    
  *
******************************************************************************/
void I2C_Start()
{
    P3DIR |= (1 << SDA); //p3.0置1.设为输出
    I2C |= (1 << SDA);//p3.0輸出1
    delay(1);
    I2C |= (1 << SCL); //p3.1输出1
    delay(1);
    I2C &= ~(1 << SDA);//p3.0置0 
    delay(1);
    I2C &= ~(1 << SCL);//p3.1 输出0
    delay(1);
}

/******************************************************************************
  * 函数名称：I2C_Stop
  * 函数描述：I2c停止信号
  * 输    入：void
  * 输    出：void
  * 返    回：void 
  * 备    注：null    
  *    
  *
******************************************************************************/
void I2C_Stop()
{
  P3DIR |= (1 << SDA);
  I2C &= ~(1 << SCL);
  I2C &= ~(1 << SDA);
  delay(1);
  I2C |= (1 << SCL);
  delay(1);
  I2C |= (1 << SDA);
  delay(1);
}

/******************************************************************************
  * 函数名称：I2C_SendACK
  * 函数描述：发送ACK
  * 输    入：void
  * 输    出：void
  * 返    回：void 
  * 备    注：null    
  *    
  *
******************************************************************************/
void I2C_SendACK(uint8_t ack)
{
  P3DIR |= (1 << SDA);
  I2C &= ~(1 << SCL);
  delay(1);
  if (!ack)
    I2C |= (1 << SDA);
  else
    I2C &= ~(1 << SDA);
  delay(1);
  I2C |= (1 << SCL);
  delay(1);
  I2C &= ~(1 << SCL);
  delay(1);
}

/******************************************************************************
  * 函数名称：I2C_RecvACK
  * 函数描述：接收ACk
  * 输    入：void
  * 输    出：void
  * 返    回：void 
  * 备    注：null    
  *    
  *
******************************************************************************/
void I2C_RecvACK()
{
  P3DIR &= ~(1 << SDA);

  I2C |= (1 << SCL);
  while ((P3IN & (1 << SDA)))
  {
    Ack_Count++;
    if (Ack_Count > 250)
    {
      I2C_Stop();
      Ack_Count = 0;
      break;
    }
  }

  I2C &= ~(1 << SCL);
  delay(1);
}

/******************************************************************************
  * 函数名称：I2C_SendByte
  * 函数描述：I2c发送一个byte
  * 输    入：
  * uint32_t dat:要发送的数据
  * 输    出：void
  * 返    回：void 
  * 备    注：null    
  *    
  *
******************************************************************************/
void I2C_SendByte(uint32_t data)
{
  uint8_t i=8;

  P3DIR |= (1 << SDA);//p3.0置1，设为输出
  I2C &= ~(1 << SCL);//p3.1输出0
  delay(1);
  while (i--) {
    if (data & 0x80)
      I2C |= (1 << SDA);//p3.0輸出1
    else
      I2C &= ~(1 << SDA); //p3.0输出0
    data <<= 1; 
    delay(1);
    I2C |= (1 << SCL);
    delay(1);
    I2C &= ~(1 << SCL);
    delay(1);
  }
  I2C |= (1 << SDA);
  I2C_RecvACK();
}

/******************************************************************************
  * 函数名称：I2C_RecvByte
  * 函数描述：I2c接收一个byte
  * 输    入：void
  * 输    出：void
  * 返    回：接收到的数据
  * 备    注：null    
  *    
  *
******************************************************************************/
uint32_t I2C_RecvByte()
{
  uint8_t i=8;
  uint32_t data = 0;

  P3DIR &= ~(1 << SDA);
  while (i--) {
    I2C &= ~(1 << SCL);
    delay(1);
    I2C|= (1 << SCL);
    data <<= 1;
    if (P3IN & (1 << SDA))
      data |= 1;
    else
      data |= 0;
    delay(1);
  }
  return data;
}

/******************************************************************************
  * 函数名称：I2C_Write_Byte
  * 函数描述：I2c往从机寄存器地址写数据
  * 输    入：
  * uint32_t Slaveaddr:从机地址
  * uint32_t REG_Address:寄存器地址
  * uint32_t REG_data：寄存器数据
  * 输    出：void
  * 返    回：void 
  * 备    注：null    
  *    
  *
******************************************************************************/
void I2C_Write_Byte(uint8_t Slaveaddr, uint8_t REG_Address, uint8_t REG_data)
{
	I2C_Start();
	I2C_SendByte(Slaveaddr);	 //模块在I2C总线中的从机地址
//	I2C_RecvACK();
	I2C_SendByte(REG_Address); 	//模块中的某个寄存器地址,设置低起始地址
//	I2C_RecvACK();
	I2C_SendByte(REG_data);	 //往寄存器中写数据
//	I2C_RecvACK();
	I2C_Stop();

}

/******************************************************************************
  * 函数名称：I2C_Read_Byte
  * 函数描述：I2c往从机寄存器读数据
  * 输    入：
  * uint32_t Slaveaddr：从机地址
  * uint32_t REG_Address:寄存器地址
  * 输    出：void
  * 返    回：读取到的数据
  * 备    注：null    
  *    
  *
******************************************************************************/
uint8_t I2C_Read_Byte(uint8_t Slaveaddr, uint8_t REG_Address)
{
  uint32_t REG_data;
  I2C_Start();                 
  I2C_SendByte(Slaveaddr);    
  I2C_SendByte(REG_Address);   
  I2C_Start();                 
  I2C_SendByte(Slaveaddr + 1); 
  REG_data = I2C_RecvByte();  
  I2C_SendACK(0);       //不再需要继续接收数据
  I2C_Stop(); 
  return REG_data;
}


/******************* (C) 版权所有 2018 北京中科浩电科技有限公司 *******************/
