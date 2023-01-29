/*
 * receiver.h
 *
 *  Created on: Dec 5, 2022
 *      Author: Jennyston
 */

#ifndef INC_DEVICES_RECEIVER_H_
#define INC_DEVICES_RECEIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include "modules/nrf24/nrf24l04.h"
#include "radioPacket_config.h"
#include "telemetry_config.h"

#define NUM_RX_CHANNELS NUM_ANALOG_CH
#define DEFAULT_RX_PIPE 0
#define TELEMETRY_RX_PIPE 1

typedef AnalogCH RxChannel;

typedef struct {
	Nrf_t radioModule;
	RadioPacket rPacket;
	//RxChannel rxChannels[NUM_RX_CHANNELS];
	Telemetry_t telemetry;
} Receiver_t;

void InitRx(Receiver_t *rx, SPI_HandleTypeDef *hspi, GPIO_t *ncs, GPIO_t *ce);
void StartRxReceive(Receiver_t *rx);
bool IsRxPacketReady(Receiver_t *rx);
void ProcessRxPacket(Receiver_t *rx);
RxChannel GetRxChannelAt(Receiver_t *rx, uint16_t index);

#endif /* INC_DEVICES_RECEIVER_H_ */
