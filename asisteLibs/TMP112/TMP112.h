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

#ifndef TMP112_H
#define TMP112_H
 
#include "mbed.h"


// Configuration Settings
#define TMP112_CFG_ONESHOT      0x8000
//resolution read only
#define TMP112_12BITS           0x6000 /*default*/
//Fault Queue
#define TMP112_CFG_1FAULT       0x0000 /*default*/
#define TMP112_CFG_2FAULT       0x0800
#define TMP112_CFG_4FAULT       0x1000
#define TMP112_CFG_6FAULT       0x1800
//alert Polarity
#define TMP112_CFG_ALERT_LOW    0x0000 /*default*/
#define TMP112_CFG_ALERT_HIGH   0x0400
//Thermostat mode
#define TMP112_CFG_TM_COMP_MODE 0x0000 /*default*/
#define TMP112_CFG_TM_INT_MODE  0x0200
//Shutdown mode
#define TMP112_CFG_ON           0x0000 /*default*/
#define TMP112_CFG_OFF          0x0100
//Convertion rate
#define TMP112_CFG_CR_0_25Hz    0x0000 
#define TMP112_CFG_CR_1Hz       0x0040
#define TMP112_CFG_CR_4Hz       0x0080 /*default*/
#define TMP112_CFG_CR_8Hz       0x00C0
//alert- Read Only
#define TMP112_CFG_ALERT        0x0020
//Extended mode
#define TMP112_CFG_EM_ON        0x0010 
#define TMP112_CFG_EM_OFF       0x0000 /*default*/
//default mode  
#define TMP112_CFG_DEF_MODE     0x60A0



// Registers to read thermopile voltage and sensor temperature

class TMP112
{
public:

    // Constructor
    TMP112(PinName sda, PinName scl, int addr);

    /** Read temperature register*/
    
    float readTemperature();
    float readTM_Htemp();
    float readTM_Ltemp();
    
    /** Read Conf registers */
    
    uint16_t readCFG();
    
    /** Set Conf options */
    
    void writeCFG(uint16_t cfg);  //give the sensor some time to power up before issuing this
    //write thermostat temperatures
    void write_TM_Htemp(float temp);
    void write_TM_Ltemp(float temp);
    

    /*!
    Destroys instance.
    */ 
    ~TMP112();
    
  
  
private:
    I2C m_i2c;
    int m_addr;
    char extended_mode;
    float readTmpReg(char reg);
    void writeTmpReg(char reg,float temp);
};
 
#endif