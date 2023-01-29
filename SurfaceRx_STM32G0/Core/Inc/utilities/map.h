/*
 * map.h
 *
 *  Created on: Dec 3, 2022
 *      Author: Jennyston
 */

#ifndef INC_UTILITIES_MAP_H_
#define INC_UTILITIES_MAP_H_

#include <stdint.h>

/**
  * @brief  maps input value to output parameters
  * @param  in input value to be mapped
  * @param	inMin minimum value of input
  * @param	inMax maximum value of input
  * @param	outMin desired minimum of output
  * @param	outMax desired maximum of output
  * @retval Returns mapped input
  **/
uint16_t map(uint16_t in, uint16_t inMin, uint16_t inMax, uint16_t outMin,
		uint16_t outMax);

#endif /* INC_UTILITIES_MAP_H_ */
