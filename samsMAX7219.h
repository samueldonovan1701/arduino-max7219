#ifndef SAMS_MAX7219
#define SAMS_MAX7219
#include "samsGPIO.h"

struct MAX7219
{
    GPIO DIN;
    GPIO CS;
    GPIO CLK;
};

//Core Functions//
void MAX7219_SetupGPIO(MAX7219 &disp);
void MAX7219_SendMSB(MAX7219 &disp, byte b);
void MAX7219_SendByte(MAX7219 &disp, byte b);
void MAX7219_SendData(MAX7219 &disp, byte addr, byte data);
void MAX7219_Clear(MAX7219 &disp);
//General Functions//
void MAX7219_TurnOff(MAX7219 &disp);
void MAX7219_TurnOn(MAX7219 &disp);
#define MAX7219_BCD_DECODING 0b11111111
#define MAX7219_NO_DECODING  0b00000000
void MAX7219_SetDecoding(MAX7219 &disp, byte mode=MAX7219_NO_DECODING);
void MAX7219_SetIntensity(MAX7219 &disp, byte intensity=0); //intensity = [0, 15]
void MAX7219_SetScanLimit(MAX7219 &disp, byte nOutputs=8); //nOutputs = [1, 8]
void MAX7219_StartTest(MAX7219 &disp);
void MAX7219_StopTest(MAX7219 &disp);


//Addresses
#define MAX7219_NO_OP_ADDR 0b0000
#define MAX7219_DIGIT_0_ADDR 0b0001
#define MAX7219_DIGIT_1_ADDR 0b0010
#define MAX7219_DIGIT_2_ADDR 0b0011
#define MAX7219_DIGIT_3_ADDR 0b0100
#define MAX7219_DIGIT_4_ADDR 0b0101
#define MAX7219_DIGIT_5_ADDR 0b0110
#define MAX7219_DIGIT_6_ADDR 0b0111
#define MAX7219_DIGIT_7_ADDR 0b1000
#define MAX7219_DECODE_MODE_ADDR 0b1001
#define MAX7219_INTENSITY_ADDR 0b1010
#define MAX7219_SCAN_LIMIT_ADDR 0b1011
#define MAX7219_SHUTDOWN_ADDR 0b1100
#define MAX7219_DISPLAY_TEST_ADDR 0b1111

//Core Functions//
void MAX7219_SetupGPIO(MAX7219 &disp)
{
	GPIO_SetMode(disp.DIN, OUTPUT);
	GPIO_SetMode(disp.CS, OUTPUT);
	GPIO_SetMode(disp.CLK, OUTPUT);
}

void MAX7219_SendMSB(MAX7219 &disp, byte b)
{
	GPIO_SetState(disp.CLK, 0);
	GPIO_SetState(disp.DIN, (b >> 7));
	GPIO_SetState(disp.CLK, 1);
}

void MAX7219_SendByte(MAX7219 &disp, byte b)
{
	MAX7219_SendMSB(disp, b << 0); //7th bit
    MAX7219_SendMSB(disp, b << 1); //6th bit
    MAX7219_SendMSB(disp, b << 2); //5th bit
    MAX7219_SendMSB(disp, b << 3); //4th bit
    MAX7219_SendMSB(disp, b << 4); //3th bit
    MAX7219_SendMSB(disp, b << 5); //2th bit
    MAX7219_SendMSB(disp, b << 6); //1th bit
    MAX7219_SendMSB(disp, b << 7); //0th bit
}

void MAX7219_SendData(MAX7219 &disp, byte addr, byte data)
{ 
    GPIO_SetState(disp.CS, 0);
	MAX7219_SendByte(disp, addr);
	MAX7219_SendByte(disp, data);
	GPIO_SetState(disp.CS, 1);
}

void MAX7219_Clear(MAX7219 &disp)
{
	MAX7219_SendData(disp, MAX7219_DIGIT_0_ADDR, 0);
	MAX7219_SendData(disp, MAX7219_DIGIT_1_ADDR, 0);
	MAX7219_SendData(disp, MAX7219_DIGIT_2_ADDR, 0);
	MAX7219_SendData(disp, MAX7219_DIGIT_3_ADDR, 0);
	MAX7219_SendData(disp, MAX7219_DIGIT_4_ADDR, 0);
	MAX7219_SendData(disp, MAX7219_DIGIT_5_ADDR, 0);
	MAX7219_SendData(disp, MAX7219_DIGIT_6_ADDR, 0);
	MAX7219_SendData(disp, MAX7219_DIGIT_7_ADDR, 0);
    MAX7219_StopTest(disp);
}

//General Functions//
//Shutdown Register//
void MAX7219_TurnOff(MAX7219 &disp)
{
	MAX7219_SendData(disp, MAX7219_SHUTDOWN_ADDR, 0b00000000);
}
void MAX7219_TurnOn(MAX7219 &disp)
{
	MAX7219_SendData(disp, MAX7219_SHUTDOWN_ADDR, 0b00000001);
}

//Decode Mode Register//
void MAX7219_SetDecoding(MAX7219 &disp, byte mode)
{
	MAX7219_SendData(disp, MAX7219_DECODE_MODE_ADDR, mode);
}

//Intensity Register//
void MAX7219_SetIntensity(MAX7219 &disp, byte intensity) //intensity = [0, 15]
{
	MAX7219_SendData(disp, MAX7219_INTENSITY_ADDR, intensity);
}

//Scan Limit Register// (Sets how many outputs are disped/written to)
void MAX7219_SetScanLimit(MAX7219 &disp, byte nOutputs) //nOutputs = [1, 8]
{
	nOutputs--;
	MAX7219_SendData(disp, MAX7219_SCAN_LIMIT_ADDR, nOutputs);
}

void MAX7219_StartTest(MAX7219 &disp)
{
	MAX7219_SendData(disp, MAX7219_DISPLAY_TEST_ADDR, 1);
}

void MAX7219_StopTest(MAX7219 &disp)
{
	MAX7219_SendData(disp, MAX7219_DISPLAY_TEST_ADDR, 0);
}

#endif
