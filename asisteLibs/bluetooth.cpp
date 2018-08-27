/**
 * @file bluetooth.cpp
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 7 jul. 2018
 */

#include "bluetooth.h"

const uint16_t Bluetooth::listaServicios[] = {
#if DEBUG
        UARTServiceShortUUID,
#else
        GattService::UUID_HEART_RATE_SERVICE,
        GattService::UUID_BATTERY_SERVICE,
        GattService::UUID_HEALTH_THERMOMETER_SERVICE,
        PedometerService::UUID_PEDOMETER_SERVICE
#endif
};

Bluetooth::Bluetooth(Leds *leds) {
    this->leds = leds;
    heartRate = 0;
    temperature = 0.0f;
    batteryLevel = 0;
    steps = 0;
    walkTime = 0;
    imu = new Imu();
    heartRateSensor = NULL;//new HeartRateSensor();
    tmp112 = new TMP112(p5, p6, 0x90);
    wait(1);
#if DEBUG
    uart = NULL;
#else
    deviceInformation = NULL;
    heartRateService = NULL;
    batteryService = NULL;
    healthThermometer = NULL;
    pedometerService = NULL;
#endif
    ble.init(this, &Bluetooth::initializedCallback);
}

Bluetooth::~Bluetooth() {
    // TODO Auto-generated destructor stub
}

void Bluetooth::startAdvertising() {
    ble.gap().startAdvertising();
    leds->startSwitchBlink();
}

void Bluetooth::waitForEvent() {
    ble.waitForEvent();
}

void Bluetooth::updateValues() {
    if(ble.getGapState().connected){
#if DEBUG
        BLEDevice mible;
        char str[200];
        sprintf(str, "ID: %.2f", tmp112->readTemperature());
        uart->writeString(str);
        ble.updateCharacteristicValue(uart->getRXCharacteristicHandle(), (uint8_t *)str, strlen(str));
#else

        heartRateService->updateHeartRate((uint8_t)heartRate);
        batteryService->updateBatteryLevel(batteryLevel);
        healthThermometer->updateTemperature(temperature);
        if(!(imu->readSteps(&steps))&&!(imu->readWalkTime(&walkTime))){
            pedometerService->updateSteps(steps);
            pedometerService->updateWalkTime(walkTime);
        }
#endif
    }
}


void Bluetooth::initializedCallback(
        BLE::InitializationCompleteCallbackContext *params) {
    if((params->ble.getInstanceID() == BLE::DEFAULT_INSTANCE) && (params->error == BLE_ERROR_NONE)){

        ble.gap().onDisconnection(this, &Bluetooth::disconnectedCallback);
        ble.gap().onConnection(this, &Bluetooth::connectedCallback);
#if DEBUG
        ble.gattServer().onDataWritten(this, &Bluetooth::dataWritenCallback);
        uart = new UARTService(ble);
#else
        heartRateService = new HeartRateService(ble, (uint8_t)0, (uint8_t)HeartRateService::LOCATION_WRIST);
        batteryService = new BatteryService(ble);
        healthThermometer = new HealthThermometerService(ble, (uint8_t)0, (uint8_t)HealthThermometerService::LOCATION_FINGER);
        uint8_t initialValue[4] = {0,0,0,0};
        pedometerService = new PedometerService(ble, initialValue, initialValue, false);
#endif
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
        ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME,
                                         (const uint8_t *)"ASISTE", sizeof("ASISTE"));
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS,
                                         (const uint8_t *)listaServicios, sizeof(listaServicios));
        ble.gap().setAdvertisingInterval(10); /* 10ms; in multiples of 0.625ms. */
        leds->setState(0b111);
    }else{

    }
}

void Bluetooth::connectedCallback(
    const Gap::ConnectionCallbackParams_t *params) {
#if DEBUG
    uart->writeString("Conectado");
#endif
    leds->stopBlink();
    leds->setState(0b010);
}

void Bluetooth::disconnectedCallback(
    const Gap::DisconnectionCallbackParams_t *params) {

}
#if DEBUG
void Bluetooth::dataWritenCallback(const GattWriteCallbackParams *params) {
    uart->writeString("Recibido");
}
#endif
