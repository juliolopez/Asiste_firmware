/**
 * @file bluetooth.h
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 7 jul. 2018
 */

#ifndef ASISTE_BLUETOOTH_H_
#define ASISTE_BLUETOOTH_H_

#include <string.h>

#include "mbed.h"
#include "BLE.h"

#include "asiste.h"

#include "UARTService.h"
#include "DeviceInformationService.h"
#include "HeartRateService.h"
#include "BatteryService.h"
#include "HealthThermometerService.h"
#include "leds.h"
#include "imu.h"
#include "heartRateSensor.h"
#include "pedometerService.h"
#include "TMP112/TMP112.h"

#define DEBUG 1

class Bluetooth {
public:
    Bluetooth(Leds *leds = new Leds());
    virtual ~Bluetooth();

    void startAdvertising();
    void waitForEvent();
    void updateValues();
private:
    Leds *leds;
    Imu *imu;
    HeartRateSensor *heartRateSensor;
    TMP112 *tmp112;
    Ticker ticker;
    BLE ble; /// Interfaz Bluetooth

    /** Definición de los servicios */
#if DEBUG
    UARTService *uart;  /// Servicio serial
#else
    DeviceInformationService *deviceInformation; /// Servicio de la información del dispositivo
    HeartRateService *heartRateService; /// Servicio del ritmo cardíaco
    BatteryService *batteryService; /// Servicio del estado de la batería
    HealthThermometerService *healthThermometer; /// Servicio del termómetro
    PedometerService *pedometerService; /// Servicio del podómetro
#endif

    static const uint16_t listaServicios[];

    uint8_t heartRate; /// Valor del ritmo cardíaco
    float temperature; /// Valor de la temperatura
    uint8_t batteryLevel; /// Valor del nivel de la batería
    uint32_t steps; /// Cuenta de los pasos
    uint32_t walkTime; /// Tiempo caminando

    /**
     * Función que se ejecuta después de que el dispositivo se haya inicializado, en ella
     * se inicializan los servicios y se establecen las funciones Callback.
     * @param params
     *          Captura el resultado de la inicialización. Se establece a BLE_ERROR_NONE
     *          si ha sido satisfactoria.
     *
     * @return void
     */
     void initializedCallback(BLE::InitializationCompleteCallbackContext *params);

     void connectedCallback(const Gap::ConnectionCallbackParams_t *params);
     void disconnectedCallback(const Gap::DisconnectionCallbackParams_t *params);

#if DEBUG
     void dataWritenCallback(const GattWriteCallbackParams *params);
#endif
};

#endif /* ASISTE_BLUETOOTH_H_ */
