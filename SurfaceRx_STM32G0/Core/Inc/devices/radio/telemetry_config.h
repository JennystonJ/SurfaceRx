/*
 * telemetry.h
 *
 *  Created on: Dec 5, 2022
 *      Author: Jennyston
 */

#ifndef INC_TELEMETRY_H_
#define INC_TELEMETRY_H_

#include <stdint.h>

typedef struct {
	float voltage;
	float speed;
	uint16_t temperature;
} Telemetry_t;

#endif /* INC_TELEMETRY_H_ */
