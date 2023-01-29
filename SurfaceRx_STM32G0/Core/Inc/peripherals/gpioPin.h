/*
 * gpio.h
 *
 *  Created on: Dec 5, 2022
 *      Author: Jennyston
 */

#ifndef INC_PERIPHERALS_GPIO_H_
#define INC_PERIPHERALS_GPIO_H_

#include "stm32f3xx_hal.h"

typedef struct{
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIO_t;

#endif /* INC_PERIPHERALS_GPIO_H_ */
