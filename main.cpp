/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "bluetooth.h"
#include "leds.h"
#include "button.h"
#include "asiste.h"


Bluetooth *bluetooth;
DigitalOut enable3v3(p30);

int main(void)
{
    enable3v3 = PWR_ON;
    wait(1);

    Leds *leds = new Leds();
    Button *button = new Button();
    leds->setState(0b111);
    Bluetooth *bluetooth = new Bluetooth(leds);
    leds->setState(0b000);
    bluetooth->startAdvertising();
//    button->attach(Callback<void()>(bluetooth, &Bluetooth::startAdvertising));
    
    while (true) {
        bluetooth->updateValues();
        bluetooth->waitForEvent();
    }
}
