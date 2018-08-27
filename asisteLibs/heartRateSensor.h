/**
 * @file HeartRateSensor.h
 * @brief 
 *
 * @author Julio López Moya
 *
 * @date 11 jul. 2018
 */

#ifndef ASISTE_HEARTRATESENSOR_H_
#define ASISTE_HEARTRATESENSOR_H_

#include "mbed.h"
#include "asiste.h"

class HeartRateSensor {
public:
    HeartRateSensor();
    virtual ~HeartRateSensor();

    uint8_t getPartId();
private:
    InterruptIn *interrupt;
};

#endif /* ASISTE_HEARTRATESENSOR_H_ */
