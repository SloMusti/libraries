/*****************************************************************************/
//	Function:	 Cpp file for class MMC7660 
//  Hardware:    Grove - 3-Axis Digital Accelerometer(±1.5g)
//	Arduino IDE: Arduino-1.0
//	Author:	 FrankieChu, modified by Musti		
//	Date: 	 Jan 10,2013, modified 25.4.2014
//	Version: v1.1
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include "MMA7660.h"

void MMA7660::write(uint8_t _register, uint8_t _data)
{
	//check if module is present
	if(notPresent()==1){
		return;
	}
	//Wire.begin();
	Wire.beginTransmission(MMA7660_ADDR);
	Wire.write(_register);   
	Wire.write(_data);
	Wire.endTransmission();
}
uint8_t MMA7660::read(uint8_t _register)
{
	//check if module is present
	if(notPresent()==1){
		return 0;
	}
	uint8_t data_read;
	//Wire.begin();
	Wire.beginTransmission(MMA7660_ADDR);
	Wire.write(_register); 
	if(Wire.endTransmission()){
		return 0;
	}
	Wire.beginTransmission(MMA7660_ADDR);
	Wire.requestFrom(MMA7660_ADDR,1);
	while(Wire.available())
	{
		data_read = Wire.read();
	}
	Wire.endTransmission();
	return data_read;
}
	
uint8_t MMA7660::notPresent(void) {
	Wire.beginTransmission(MMA7660_ADDR);
	Wire.write(0x00);
	uint8_t err = Wire.endTransmission();
	return err;
}

void MMA7660::init()
{
	setMode(MMA7660_STAND_BY);
	setSampleRate(AUTO_SLEEP_120);
	setMode(MMA7660_ACTIVE);
}
void MMA7660::setMode(uint8_t mode)
{
	write(MMA7660_MODE,mode);
}
void MMA7660::setSampleRate(uint8_t rate)
{
	write(MMA7660_SR,rate);
}
void MMA7660::getXYZ(int8_t *x,int8_t *y,int8_t *z)
{
	unsigned char val[3];
	
	Wire.beginTransmission(MMA7660_ADDR);
    Wire.write(MMA7660_X);
    Wire.endTransmission();
    uint8_t receivedBytes = Wire.requestFrom(MMA7660_ADDR,(uint8_t) 3);    // need to cast int to avoid compiler warnings
	uint8_t *ptr = (uint8_t *) val;
  	while(Wire.available()){
  		*ptr++ = Wire.read();
  	}
	Wire.endTransmission();

	*x = (int8_t)(val[0]<<2);
	*x /=4;
  	*y = (int8_t)(val[1]<<2);
	*y /=4;
  	*z = (int8_t)(val[2]<<2);
	*z /=4;
}
void MMA7660::getAcclemeter(float *ax,float *ay,float *az)
{
	int8_t x,y,z;
	getXYZ(&x,&y,&z);
	*ax = x/21.00;
	*ay = y/21.00;
	*az = z/21.00;
}

