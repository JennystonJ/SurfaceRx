/*
 * radioField.h
 *
 *  Created on: Aug 29, 2023
 *      Author: Jennyston
 */

#ifndef INC_DEVICES_RADIO_RADIOFIELD_H_
#define INC_DEVICES_RADIO_RADIOFIELD_H_

#include <stdint.h>

typedef enum {
	FIELD_ANALOG_CH,
	FIELD_DIGITAL_CH,
} RadioFieldType;

typedef struct {
	RadioFieldType fieldType;
	uint16_t fieldData;
} RadioField;

#endif /* INC_DEVICES_RADIO_RADIOFIELD_H_ */
