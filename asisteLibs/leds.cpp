/**
 * @file leds.cpp
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 8 jul. 2018
 */

#include "leds.h"

Leds::Leds() {
    ledsToBlink = 0b000;
    green = new DigitalOut(LED1, LED_OFF);
    red = new DigitalOut(LED2, LED_OFF);
    blue = new DigitalOut(LED3, LED_OFF);
}

Leds::~Leds() {
    // TODO Auto-generated destructor stub
}

void Leds::setState(uint8_t RGB) {
    if(RGB & 0b100) *this->red = LED_ON; else *this->red = LED_OFF;
    if(RGB & 0b010) *this->green = LED_ON; else *this->green = LED_OFF;
    if(RGB & 0b001) *this->blue = LED_ON; else *this->blue = LED_OFF;
}

void Leds::startBlink(uint8_t RGB, us_timestamp_t us) {
    setState(0b000);
    ledsToBlink = RGB;
    ticker.attach_us(Callback<void()>(this, &Leds::blinkCallback), us);
}

void Leds::startSwitchBlink() {
    ticker.detach();
    setState(0b100);
    ticker.attach_us(Callback<void()>(this, &Leds::switchBlinkCallback), 250000);
}

void Leds::stopBlink() {
    ticker.detach();
    setState(0b000);
}

void Leds::blinkCallback(){
    if(ledsToBlink & 0b100) *red = *red ^ 1;
    if(ledsToBlink & 0b010) *green = *green ^ 1;
    if(ledsToBlink & 0b001) *blue = *blue ^ 1;
}

void Leds::switchBlinkCallback() {
    *red = *red ^ 1;
    *blue = *blue ^ 1;
}
