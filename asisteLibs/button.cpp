/**
 * @file button.cpp
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 8 jul. 2018
 */

#include "button.h"

Button::Button() {
    // TODO Auto-generated constructor stub
    button = new InterruptIn(p17);
    button->fall(Callback<void()>(this, &Button::buttonCallback));
    tap = NULL;
}

Button::~Button() {
    // TODO Auto-generated destructor stub
}

void Button::attach(Callback<void()> func) {
    core_util_critical_section_enter();
    tap = func;
    core_util_critical_section_exit();
}

void Button::buttonCallback() {
    ticker.attach_us(Callback<void()>(this, &Button::debounceCallback),100000);
}

void Button::debounceCallback() {
    static uint8_t counterLows = 0;
    static uint8_t counterEntries = 0;

    counterEntries++;
    if(!button->read()) counterLows++;
    if(counterEntries > 10){
        counterEntries = 0;
        ticker.detach();
        if(counterLows > 8){
            counterLows = 0;
            tap();
        }
        counterLows = 0;
    }
}
