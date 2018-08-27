/**
 * @file leds.h
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 8 jul. 2018
 */

#ifndef ASISTE_LEDS_H_
#define ASISTE_LEDS_H_

#include "mbed.h"

#include "asiste.h"

class Leds {
public:
    Leds();
    virtual ~Leds();

    void setState(uint8_t RGB);
    void startBlink(uint8_t RGB, us_timestamp_t us);
    void startSwitchBlink();
    void stopBlink();
private:
    uint8_t ledsToBlink;
    DigitalOut *green;
    DigitalOut *red;
    DigitalOut *blue;

    Ticker ticker;

    void blinkCallback();

    void switchBlinkCallback();
};

#endif /* ASISTE_LEDS_H_ */
