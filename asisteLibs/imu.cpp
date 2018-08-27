/**
 * @file imu.cpp
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 13 jul. 2018
 */

#include "imu.h"

static signed char board_orientation[9]={
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
};

void tap_cb(unsigned char direction, unsigned char count)
{

}
void android_orient_cb(unsigned char orientation)
{

}

unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;
    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;
    return scalar;
}

Imu::Imu() {
    motionEvent = 0;
    imuInterrupt = new InterruptIn(MPU6050_INT_PIN);
    mbed_i2c_clear(MPU6050_SDA_PIN, MPU6050_SCL_PIN);
    mbed_i2c_init(MPU6050_SDA_PIN, MPU6050_SCL_PIN);
    while(mpu_init(0));
    /* Get/set hardware configuration. Start gyro. */
    /* Wake up all sensors. */
    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    /* Push both gyro and accel data into the FIFO. */
    mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    mpu_set_sample_rate(DEFAULT_MPU_HZ);

    /* Read back configuration in case it was set improperly. */
    unsigned char accel_fsr;
    unsigned short gyro_rate, gyro_fsr;
    mpu_get_sample_rate(&gyro_rate);
    mpu_get_gyro_fsr(&gyro_fsr);
    mpu_get_accel_fsr(&accel_fsr);

    dmp_load_motion_driver_firmware();
    dmp_set_orientation(
        inv_orientation_matrix_to_scalar(board_orientation));
    dmp_register_tap_cb(tap_cb);
    dmp_register_android_orient_cb(android_orient_cb);

    uint16_t dmp_features = DMP_FEATURE_TAP | DMP_FEATURE_PEDOMETER;
    dmp_enable_feature(dmp_features);
    dmp_set_fifo_rate(DEFAULT_MPU_HZ);
    mpu_set_dmp_state(1);

    dmp_set_interrupt_mode(DMP_INT_GESTURE);
    dmp_set_tap_thresh(TAP_XYZ, 50);
    imuInterrupt->fall(Callback<void()>(this, &Imu::imuCallback));
}

Imu::~Imu() {
    // TODO Auto-generated destructor stub
}

uint8_t Imu::getMotionEvent() const {
    return motionEvent;
}

void Imu::readFifo() {
}

int Imu::readSteps(unsigned long *steps) {
    return dmp_get_pedometer_step_count(steps);
}

int Imu::readWalkTime(unsigned long *walkTime) {
    return dmp_get_pedometer_walk_time(walkTime);
}

void Imu::setMotionEvent(uint8_t motionEvent) {
    this->motionEvent = motionEvent;
}

void Imu::imuCallback() {
    motionEvent = 1;
}
