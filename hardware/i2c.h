#ifndef _I2C_H
#define _I2C_H
//�ⲿ�ļ�����

//�궨����
#define SDA 0
#define SCL 1
#define I2C P3OUT

//��������
uint8_t I2C_Read_Byte(uint8_t Slaveaddr, uint8_t REG_Address);
void I2C_Write_Byte(uint8_t Slaveaddr, uint8_t REG_Address, uint8_t REG_data);
void I2C_Init();


#endif


/******************* (C) ��Ȩ���� 2018 �����пƺƵ�Ƽ����޹�˾ *******************/
