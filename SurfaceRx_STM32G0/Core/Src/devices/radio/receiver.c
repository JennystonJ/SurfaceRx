/*
 * receiver.c
 *
 *  Created on: Dec 5, 2022
 *      Author: Jennyston
 */

#include "devices/radio/receiver.h"
#include "devices/radio/modules/nrf24/nrf24l04.h"
#include "assert.h"

void InitRx(Receiver_t *rx, SPI_HandleTypeDef *hspi, GPIO_t *ncs, GPIO_t *ce) {
	ASSERT(rx != NULL);
	ASSERT(hspi != NULL);
	InitNrf(&rx->radioModule, hspi, ncs, ce);

	//configure default payload width to match channel packet size
	uint8_t data = sizeof(rx->rPacket);
	ASSERT(data <= MAX_PACKET_LENGTH);
	WriteNrfRegister(&rx->radioModule, NRF_RX_PW_P0 + DEFAULT_RX_PIPE,
			1, &data);

	//configure telemetry payload width to match telemetry packet size
	data = sizeof(rx->telemetry);
	ASSERT(data <= MAX_PACKET_LENGTH);
	WriteNrfRegister(&rx->radioModule, NRF_RX_PW_P0 + TELEMETRY_RX_PIPE,
			1, &data);

	//only enable data received interrupts, mask others
	data = 0b00111011;
	WriteNrfRegister(&rx->radioModule, NRF_CONFIG, 1, &data);
}

void WriteRxTelemetry(Receiver_t *rx) {
	ASSERT(rx != NULL);
	ASSERT(sizeof(rx->telemetry) <= MAX_PACKET_LENGTH);
	WriteNrfAckPayload(&rx->radioModule, 1, (uint8_t *)(&rx->telemetry),
			sizeof(rx->telemetry));
}

void StartRxReceive(Receiver_t *rx) {
	ASSERT(rx != NULL);
	StartNrfPRX(&rx->radioModule);
}

bool IsRxPacketReady(Receiver_t *rx) {
	ASSERT(rx != NULL);
	return IsNrfDataReady(&rx->radioModule);
}

void ProcessRxPacket(Receiver_t *rx) {
	ASSERT(rx != NULL);
	ASSERT(sizeof(rx->rPacket) <= MAX_PACKET_LENGTH);
	ReadNrfRxPayload(&rx->radioModule, (uint8_t *)(&rx->rPacket),
			sizeof(rx->rPacket));

	//clear data ready flag
	uint8_t data = 0b01000000;
	WriteNrfRegister(&rx->radioModule, NRF_STATUS, 1, &data);
}

RxChannel GetRxChannelAt(Receiver_t *rx, uint16_t index) {
	ASSERT(rx != NULL);
	ASSERT(index < NUM_ANALOG_CH);
	return rx->rPacket.analogCh[index];
}
