/*
Copyright (c) 2016 Camilo Ruiz 
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

//this should work with tmp102 too
//I do not use negative values on my application, this code has to be tweaked to support it
//I do not use extended mode, this code has to be tweaked to support it

//greetings from Colombia :)

#include "TMP112.h"

#define TEMP_REG_ADDR 0x00

TMP112::TMP112(PinName sda, PinName scl, int addr) : m_i2c(sda, scl), m_addr(addr)
{
    extended_mode=0; //default, extended mode off
}

TMP112::~TMP112()
{
}


/** TMP112 read one of the three available temperature registers
 *  return in celsius 
 */
float TMP112::readTmpReg(char reg)
{
    m_i2c.write(m_addr, &reg, 1); //Pointer to the temperature register
    char data[2] = {0,0};
    m_i2c.read(m_addr, data, 2); //Read
    int16_t res  = ((int8_t)data[0] << 4) | ((uint8_t)data[1] >> 4);  
    float temp =  (float) ((float)res * 0.0625);
    return temp;
}

/** TMP112 get the actual temperature of the sensor 
 */
float TMP112::readTemperature()
{
    return readTmpReg(0);
}

/** TMP112 read the hi temp of the thermostat stored in the sensor
 */
float TMP112::readTM_Htemp()
{
    return readTmpReg(3);
}

/** TMP112 read the low temp of the thermostat stored in the sensor
 */
float TMP112::readTM_Ltemp()
{
    return readTmpReg(2);
}

/** TMP112 read the cfg register
 */
uint16_t TMP112::readCFG()
{
    char reg=1;
    m_i2c.write(m_addr, &reg, 1); //Pointer to the cfg register
    char data[2] = {0,0};
    m_i2c.read(m_addr, data, 2); //Read
    return (data[0] << 8) | data[1];
}

/** TMP112 write the cfg register
 */
void TMP112::writeCFG(uint16_t cfg)
{
    //char reg=1;
    //m_i2c.write(m_addr, &reg, 1); //Pointer to the cfg register
    char data[3];
    data[0]=1;      //Pointer to the cfg register
    data[1]=cfg>>8;
    data[2]=cfg & 0xFF;
    m_i2c.write(m_addr, data, 3); //write
}
/** TMP112 write a threshold temp o thermostat into the sensor
 */
void TMP112::writeTmpReg(char reg, float temp){
    uint16_t regValue= (uint16_t) (temp/0.0625);
    char data[3];
    data[0]=reg;      //Pointer to the cfg register
    data[1]=(regValue>>4)& 0xFF;
    data[2]=(regValue << 4) & 0xFF;
    m_i2c.write(m_addr, data, 3); //write
}

//write thermostat temperatures high
    void TMP112::write_TM_Htemp(float temp){
        writeTmpReg(3,temp);
        }
//write thermostat temperatures low        
    void TMP112::write_TM_Ltemp(float temp){
        writeTmpReg(2,temp);
        }
    