/**
 * @file button.h
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 8 jul. 2018
 */

#ifndef ASISTE_BUTTON_H_
#define ASISTE_BUTTON_H_

#include "mbed.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_sleep.h"

#include "asiste.h"

class Button {
public:
    Button();
    virtual ~Button();

    void attach(Callback<void()> func);

private:
    Ticker ticker;
    InterruptIn *button;

    void buttonCallback();
    void debounceCallback();

    Callback<void()> tap;

protected:
};

#endif /* ASISTE_BUTTON_H_ */
