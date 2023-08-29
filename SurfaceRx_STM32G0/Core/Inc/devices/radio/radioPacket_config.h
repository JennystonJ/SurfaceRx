/*
 * radioPacket_config.h
 *
 *  Created on: Dec 6, 2022
 *      Author: Jennyston
 */

#ifndef INC_DEVICES_RADIO_PACKET_CONFIG_H_
#define INC_DEVICES_RADIO_PACKET_CONFIG_H_

#define NUM_ANALOG_CH 2
#define NUM_DIGITAL_CH 0

#define ANALOG_CH_MIN 0
#define ANALOG_CH_MAX 1023

#define NUM_CHANNELS 6

#define NUM_FIELDS NUM_CHANNELS

#define DIGITAL_CH_MIN 0
#define DIGITAL_CH_MAX 2

#include "radioField.h"

//analog channels are handled as 10-bit
typedef uint16_t AnalogCH;
//digital channels are handled as 1-bit
typedef uint8_t DigitalCH;

typedef enum {
	PACKET_TYPE_CONFIG,
	PACKET_TYPE_DATA
} RadioPacketType;

typedef struct {
	RadioPacketType packetType;
	RadioField field[NUM_FIELDS];
	AnalogCH analogCh[NUM_ANALOG_CH];
} RadioPacket;



#endif /* INC_DEVICES_RADIO_PACKET_CONFIG_H_ */
