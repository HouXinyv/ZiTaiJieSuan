#include "driverlib.h"
#include "i2c.h"
#include "mpu6050.h"
#include "stdint.h"

int main(void)
{
  volatile static uint8_t buff = 0;
  volatile static uint8_t buffer[14];
  int i;
  WDTCTL = WDTPW + WDTHOLD;
  I2C_Init();
  MPU6050Init();
  buff = I2C_Read_Byte(MPU6050_ADDRESS, 0x75);
  while(1) {
    for(i = 0; i < 14; i++) {
      buffer[i] = I2C_Read_Byte(MPU6050_ADDRESS, 0x3B + i);
    }
    __delay_cycles(1000); 
  } 
}