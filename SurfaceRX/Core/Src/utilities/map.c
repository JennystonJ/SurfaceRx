/*
 * map.c
 *
 *  Created on: Dec 4, 2022
 *      Author: Jennyston
 */

#include "utilities/map.h"
#include "assert.h"

uint16_t map(uint16_t in, uint16_t inMin, uint16_t inMax, uint16_t outMin,
		uint16_t outMax) {
	//input saturation filter
	if(in < inMin) {
		in = inMin;
	}
	else if(in > inMax) {
		in = inMax;
	}

	//find ranges
	uint16_t rangeIn = inMax - inMin;
	uint16_t rangeOut = outMax - outMin;

	//scale and map input
	uint16_t result = (uint16_t)(((float)rangeOut/rangeIn) *
			(in-inMin) + outMin);

	//output saturation filter
	if(result < outMin) {
		result = outMin;
	}
	else if(result > outMax) {
		result = outMax;
	}

	return result;
}
