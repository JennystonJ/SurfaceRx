#include "devices/radio/modules/nrf24/nrf24l04.h"
#include "utilities/delay.h"
#include "assert.h"

//SPI Commands
//TODO: Convert to enumerated type
#define R_Rx_Payload 0b01100001
#define W_Tx_Payload 0b10100000
#define Flush_Tx 0b11100001
#define Flush_Rx 0b11100010
#define Reuse_Tx_Pl 0b11100011
#define R_Rx_Pl_Wid 0b01100000
#define W_Ack_Payload 0b10101000
#define W_Tx_Payload_Noack 0b10110000
#define NOP 0b11111111

static void SetNrfCE(Nrf_t *nrf, bool set);
static void AssertNrfCS(Nrf_t *nrf, bool set);


void InitNrf(Nrf_t *nrf, SPI_HandleTypeDef *hspi, GPIO_t *ncs,
		GPIO_t *ce){
    //initialize nrf parameters
	nrf->hspi = hspi;
	nrf->ncsGpio = ncs;
	nrf->ceGpio = ce;

    uint8_t data;
    //500 us auto retransmit delay (minimum required for 250 Kbps)
    //up to 15 retransmits on transmission fail
    data = 0b00010000 | 0b00001111;
    WriteNrfRegister(nrf, NRF_SETUP_RETR, 1, &data);

    //set RF data rate to 250 Kbps
    //low output power
    data = 0b00100010;
    WriteNrfRegister(nrf, NRF_RF_SETUP, 1, &data);

    //flush transmit and receive buffers
    FlushNrfTx(nrf);
    FlushNrfRx(nrf);

    //clear max retry, data sent, and data received flags
    data = 0b01110000;
    WriteNrfRegister(nrf, NRF_STATUS, 1, &data);

    /* keep all interrupts enabled
     * 1-byte CRC
     * power on
     * primary TX
     */
    data = 0b00001010;
    WriteNrfRegister(nrf, NRF_CONFIG,  1, &data);

}

void StartNrfPRX(Nrf_t *nrf) {
    //read current configuration
	uint8_t data;
    ReadNrfRegister(nrf, NRF_CONFIG, 1, &data);

    /* preserve existing configuration
     * power on
     * primary RX
     */
    data = data | 0b00000011;
    WriteNrfRegister(nrf, NRF_CONFIG, 1, &data);

    //listen for packets
    SetNrfCE(nrf, true);

}

void StopNrfPRX(Nrf_t *nrf) {
    SetNrfCE(nrf, false);
}

void TransmitNrfPacket(Nrf_t *nrf) {
	SetNrfCE(nrf, true);
	//wait 15 us after asserting CE
	DelayUs(15);
	//release CE
	SetNrfCE(nrf, false);
}

bool IsNrfDataReady(Nrf_t *nrf) {
    uint8_t status = NopNrf(nrf);
    bool dataReady = (bool)((status >> 6) & 1);
    return dataReady;
}

bool IsNrfDataSent(Nrf_t *nrf) {
	uint8_t status = NopNrf(nrf);
	bool dataSent = (bool)((status >> 5) & 1);
	return dataSent;
}

void ReadNrfRegister(Nrf_t *nrf, NrfReg reg, uint8_t size,
        uint8_t *data) {

    AssertNrfCS(nrf, true);

    //initiate read operation
    uint8_t status;
    HAL_SPI_TransmitReceive(nrf->hspi, &reg, &status, 1,
    		HAL_MAX_DELAY);
    //read
    HAL_SPI_Receive(nrf->hspi, data, size, HAL_MAX_DELAY);
//    for(int i = 0; i < size; i++) {
//        *(data+i) = nrfSpi.ExchangeData8(0);
//    }

    AssertNrfCS(nrf, false);
}

void WriteNrfRegister(Nrf_t *nrf, NrfReg reg, uint8_t size,
        uint8_t *data) {

	AssertNrfCS(nrf, true);

    //initiate write operation
	uint8_t status;
	uint8_t wReg = (uint8_t)reg | 0b00100000;
	HAL_SPI_TransmitReceive(nrf->hspi, &wReg, &status, 1,
			HAL_MAX_DELAY);
    //write
//	HAL_SPI_TransmitReceive(nrf->hspi, data, &status, size,
//    			HAL_MAX_DELAY);
	HAL_SPI_Transmit(nrf->hspi, data, size, HAL_MAX_DELAY);

	AssertNrfCS(nrf, false);
}

void ReadNrfRxPayload(Nrf_t *nrf, uint8_t *data, uint8_t size) {

	ASSERT(size <= 32);
	AssertNrfCS(nrf, true);

    //initiate RX payload read
    uint8_t txData = R_Rx_Payload;
    HAL_SPI_TransmitReceive(nrf->hspi, &txData, data, 1, HAL_MAX_DELAY);
    //read RX payload
    HAL_SPI_Receive(nrf->hspi, data, size, HAL_MAX_DELAY);

	AssertNrfCS(nrf, false);
}

void WriteNrfTxPayload(Nrf_t *nrf, uint8_t *data, uint8_t size) {

	ASSERT(size <= 32);
	AssertNrfCS(nrf, true);


    //initiate TX payload write
	uint8_t status;
    uint8_t txData = W_Tx_Payload;
    HAL_SPI_TransmitReceive(nrf->hspi, &txData, &status, 1, HAL_MAX_DELAY);
    //write TX payload
    HAL_SPI_Transmit(nrf->hspi, data, size, HAL_MAX_DELAY);

	AssertNrfCS(nrf, false);
}

void WriteNrfAckPayload(Nrf_t *nrf, uint8_t pipe, uint8_t *data,
		uint8_t size) {

	ASSERT(pipe <= 5);
	ASSERT(size <= MAX_PACKET_LENGTH);

	AssertNrfCS(nrf, true);

	//initiate ACK payload write
	uint8_t status;
	uint8_t command = W_Ack_Payload | pipe;
	HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);
	//write ACK payload
	HAL_SPI_Transmit(nrf->hspi, data, size, HAL_MAX_DELAY);

	AssertNrfCS(nrf, false);
}

void FlushNrfTx(Nrf_t *nrf) {

	AssertNrfCS(nrf, true);

    //flush TX FIFO
	uint8_t status;
	uint8_t command = Flush_Tx;
	HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);

	AssertNrfCS(nrf, false);
}

void FlushNrfRx(Nrf_t *nrf) {

	AssertNrfCS(nrf, true);

    //flush RX FIFO
	uint8_t status;
	uint8_t command = Flush_Rx;
	HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);

	AssertNrfCS(nrf, false);
}

void ReuseNrfTxPayload(Nrf_t *nrf) {

	AssertNrfCS(nrf, true);

    //reuse last transmitted payload
    uint8_t status;
    uint8_t txData = Reuse_Tx_Pl;
    HAL_SPI_TransmitReceive(nrf->hspi, &txData, &status, 1, HAL_MAX_DELAY);

	AssertNrfCS(nrf, false);
}

uint8_t ReadNrfRxPayloadWidth(Nrf_t *nrf) {

	AssertNrfCS(nrf, true);

    //read RX payload width for top payload
	uint8_t status;
	uint8_t command = R_Rx_Pl_Wid;
    HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);
    uint8_t payloadWidth;
    HAL_SPI_Receive(nrf->hspi, &payloadWidth, 1, HAL_MAX_DELAY);

	AssertNrfCS(nrf, false);
    return payloadWidth;
}

void WriteNrfTxPayloadNoAck(Nrf_t *nrf, uint8_t *data, uint8_t size) {

	AssertNrfCS(nrf, true);

    uint8_t status;
    uint8_t command = W_Tx_Payload_Noack;
    HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(nrf->hspi, data, size, HAL_MAX_DELAY);

	AssertNrfCS(nrf, false);
}

uint8_t NopNrf(Nrf_t *nrf) {

	AssertNrfCS(nrf, true);

    uint8_t status;
    uint8_t command = NOP;
    HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);

	AssertNrfCS(nrf, false);

	return status;
}

static void SetNrfCE(Nrf_t *nrf, bool set) {
	HAL_GPIO_WritePin(nrf->ceGpio->port, nrf->ceGpio->pin,
			set ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void AssertNrfCS(Nrf_t *nrf, bool set) {
	HAL_GPIO_WritePin(nrf->ncsGpio->port, nrf->ncsGpio->pin,
			set ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
