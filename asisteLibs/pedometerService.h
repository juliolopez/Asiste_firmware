/**
 * @file pedometerService.h
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 13 jul. 2018
 */

#ifndef ASISTE_PEDOMETERSERVICE_H_
#define ASISTE_PEDOMETERSERVICE_H_

#include <string.h>

#include "mbed.h"
#include "BLE.h"

#include "leds.h"

class PedometerService {
public:
    const static uint16_t UUID_PEDOMETER_SERVICE = 0xA020;
    const static uint16_t UUID_PEDOMETER_STEPS = 0xA021;
    const static uint16_t UUID_PEDOMETER_WALKTIME = 0xA022;
    const static uint16_t UUID_PEDOMETER_RESET = 0xA023;

    PedometerService(BLE &_ble, uint8_t initialStepsCounter[4], uint8_t initialWalkTime[4], bool reset);
    virtual ~PedometerService();

    void updateSteps(uint32_t steps);
    void updateWalkTime(uint32_t walkTime);
    GattAttribute::Handle_t getReset() const;


private:
    BLE &ble;
    uint32_t steps;
    uint32_t walkTime;
    ReadOnlyArrayGattCharacteristic<uint8_t, sizeof(steps)> stepsCounter;
    ReadOnlyArrayGattCharacteristic<uint8_t, sizeof(walkTime)> walkTimeMeassurement;
    WriteOnlyGattCharacteristic <bool> resetSignal;

};

#endif /* ASISTE_PEDOMETERSERVICE_H_ */
