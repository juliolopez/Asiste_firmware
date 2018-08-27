/**
 * @file imu.h
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 13 jul. 2018
 */

#ifndef ASISTE_IMU_H_
#define ASISTE_IMU_H_

#include "mbed.h"
#include "mbed_i2c.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#include "asiste.h"

#define DEFAULT_MPU_HZ 100

void tap_cb(unsigned char direction, unsigned char count);
unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
unsigned short inv_row_2_scale(const signed char *row);
void android_orient_cb(unsigned char orientation);

class Imu {
public:
    Imu();
    virtual ~Imu();
    uint8_t getMotionEvent() const;
    void readFifo();
    int readSteps(unsigned long *steps);
    int readWalkTime(unsigned long *walkTime);
    void setMotionEvent(uint8_t motionEvent);

private:
    InterruptIn *imuInterrupt;
    uint8_t motionEvent;

    void imuCallback();
};

#endif /* ASISTE_IMU_H_ */
