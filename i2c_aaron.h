/* I2C_Aaron.h
*
* Author: Aaron Becker
* For COSMOS Cluster 6 2019
*
*/

/*
Basics of using PIC32:
Tris register sets pin input or output. 
Set as 1 -> input
0 -> output
Default intput

Can read from PORT registers
Can write to LAT registers
*/

#define I2C_SDAOUT PORTX;
#define I2C_SCLOUT PORTY;
#define I2C_SDAIN PORTV;
#define I2C_SCLIN PORTW;


#ifndef I2C_DRIVER_AARON_H //include guard
#define I2C_DRIVER_AARON_H

int I2CAaron_Init(unsigned int speed); //init function taking speed as a value in Hz
int I2CAaron_ReadInt(char I2Caddress, char deviceRegister); //read a single int from address & register
unsigned char I2C_ReadReg(char I2Caddress, char deviceRegister);
unsigned char I2C_WriteReg(char I2Caddress, char deviceRegister, char data);
void I2C_ReadMultiple(char I2Caddress, char deviceRegister, char *outArray, int numBytes)

void internal_startSeq(char I2Caddress);
void internal_stopSeq(char I2Caddress);


#endif