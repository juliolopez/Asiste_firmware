/**
 * @file pedometerService.cpp
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 13 jul. 2018
 */

#include "pedometerService.h"

PedometerService::PedometerService(BLE &_ble, uint8_t initialStepsCounter[4], uint8_t initialWalkTime[4], bool reset = 0) :
    ble(_ble),
    stepsCounter (UUID_PEDOMETER_STEPS, initialStepsCounter, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
    walkTimeMeassurement (UUID_PEDOMETER_WALKTIME, initialWalkTime, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
    resetSignal (UUID_PEDOMETER_RESET, &reset){
    steps = 0;
    walkTime = 0;
    GattCharacteristic *charTable[] = {&stepsCounter, &walkTimeMeassurement, &resetSignal};
    GattService pedometerService(PedometerService::UUID_PEDOMETER_SERVICE, charTable, sizeof(charTable)/sizeof(GattCharacteristic *));
    ble.gattServer().addService(pedometerService);
}

PedometerService::~PedometerService() {
    // TODO Auto-generated destructor stub
}

void PedometerService::updateSteps(uint32_t steps) {
    uint8_t array[4];
    array[0] = steps & 0xff000000;
    array[1] = steps & 0x00ff0000;
    array[2] = steps & 0x0000ff00;
    array[3] = steps & 0x000000ff;
    ble.gattServer().write(stepsCounter.getValueHandle(), array, sizeof(array));
}

void PedometerService::updateWalkTime(uint32_t walkTime) {
    uint8_t array[4] = {
                walkTime & 0xff000000,
                walkTime & 0x00ff0000,
                walkTime & 0x0000ff00,
                walkTime & 0x000000ff
        };
    ble.gattServer().write(walkTimeMeassurement.getValueHandle(),array, sizeof(steps));
}

GattAttribute::Handle_t PedometerService::getReset() const {
    return resetSignal.getValueHandle();
}
