#include "devices/radio/modules/nrf24/nrf24l04.h"
#include "devices/radio/modules/nrf24/nrf24l01_cmd.h"
#include "utilities/delay.h"
#include "assert.h"

static void SetNrfCE(Nrf_t *nrf, bool set);
static void SelectNrfCS(Nrf_t *nrf, bool set);


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

    SelectNrfCS(nrf, true);

    //initiate read operation
    uint8_t status;
    HAL_SPI_TransmitReceive(nrf->hspi, &reg, &status, 1,
    		HAL_MAX_DELAY);
    //read
    HAL_SPI_Receive(nrf->hspi, data, size, HAL_MAX_DELAY);
//    for(int i = 0; i < size; i++) {
//        *(data+i) = nrfSpi.ExchangeData8(0);
//    }

    SelectNrfCS(nrf, false);
}

void WriteNrfRegister(Nrf_t *nrf, NrfReg reg, uint8_t size,
        uint8_t *data) {

	SelectNrfCS(nrf, true);

    //initiate write operation
	uint8_t status;
	uint8_t wReg = (uint8_t)reg | 0b00100000;
	HAL_SPI_TransmitReceive(nrf->hspi, &wReg, &status, 1,
			HAL_MAX_DELAY);
    //write
//	HAL_SPI_TransmitReceive(nrf->hspi, data, &status, size,
//    			HAL_MAX_DELAY);
	HAL_SPI_Transmit(nrf->hspi, data, size, HAL_MAX_DELAY);

	SelectNrfCS(nrf, false);
}

void ReadNrfRxPayload(Nrf_t *nrf, uint8_t *data, uint8_t size) {

	ASSERT(size <= 32);
	SelectNrfCS(nrf, true);

    //initiate RX payload read
    NrfCmd command = NRF_CMD_R_RX_PAYLOAD;
    HAL_SPI_TransmitReceive(nrf->hspi, &command, data, 1, HAL_MAX_DELAY);
    //read RX payload
    HAL_SPI_Receive(nrf->hspi, data, size, HAL_MAX_DELAY);

	SelectNrfCS(nrf, false);
}

void WriteNrfTxPayload(Nrf_t *nrf, uint8_t *data, uint8_t size) {

	ASSERT(size <= 32);
	SelectNrfCS(nrf, true);


    //initiate TX payload write
	uint8_t status;
    NrfCmd command = NRF_CMD_W_TX_PAYLOAD;
    HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);
    //write TX payload
    HAL_SPI_Transmit(nrf->hspi, data, size, HAL_MAX_DELAY);

	SelectNrfCS(nrf, false);
}

void WriteNrfAckPayload(Nrf_t *nrf, uint8_t pipe, uint8_t *data,
		uint8_t size) {

	ASSERT(pipe <= 5);
	ASSERT(size <= MAX_PACKET_LENGTH);

	SelectNrfCS(nrf, true);

	//initiate ACK payload write
	uint8_t status;
	NrfCmd command = NRF_CMD_W_ACK_PAYLOAD | pipe;
	HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);
	//write ACK payload
	HAL_SPI_Transmit(nrf->hspi, data, size, HAL_MAX_DELAY);

	SelectNrfCS(nrf, false);
}

void FlushNrfTx(Nrf_t *nrf) {

	SelectNrfCS(nrf, true);

    //flush TX FIFO
	uint8_t status;
	NrfCmd command = NRF_CMD_FLUSH_TX;
	HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);

	SelectNrfCS(nrf, false);
}

void FlushNrfRx(Nrf_t *nrf) {

	SelectNrfCS(nrf, true);

    //flush RX FIFO
	uint8_t status;
	NrfCmd command = NRF_CMD_FLUSH_RX;
	HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);

	SelectNrfCS(nrf, false);
}

void ReuseNrfTxPayload(Nrf_t *nrf) {

	SelectNrfCS(nrf, true);

    //reuse last transmitted payload
    uint8_t status;
    NrfCmd command = NRF_CMD_REUSE_TX_PL;
    HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);

	SelectNrfCS(nrf, false);
}

uint8_t ReadNrfRxPayloadWidth(Nrf_t *nrf) {

	SelectNrfCS(nrf, true);

    //read RX payload width for top payload
	uint8_t status;
	NrfCmd command = NRF_CMD_R_RX_PL_WID;
    HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);
    uint8_t payloadWidth;
    HAL_SPI_Receive(nrf->hspi, &payloadWidth, 1, HAL_MAX_DELAY);

	SelectNrfCS(nrf, false);
    return payloadWidth;
}

void WriteNrfTxPayloadNoAck(Nrf_t *nrf, uint8_t *data, uint8_t size) {

	SelectNrfCS(nrf, true);

    uint8_t status;
    NrfCmd command = NRF_CMD_W_TX_PAYLOAD_NOACK;
    HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(nrf->hspi, data, size, HAL_MAX_DELAY);

	SelectNrfCS(nrf, false);
}

uint8_t NopNrf(Nrf_t *nrf) {

	SelectNrfCS(nrf, true);

    uint8_t status;
    NrfCmd command = NRF_CMD_NOP;
    HAL_SPI_TransmitReceive(nrf->hspi, &command, &status, 1, HAL_MAX_DELAY);

	SelectNrfCS(nrf, false);

	return status;
}

static void SetNrfCE(Nrf_t *nrf, bool set) {
	HAL_GPIO_WritePin(nrf->ceGpio->port, nrf->ceGpio->pin,
			set ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void SelectNrfCS(Nrf_t *nrf, bool set) {
	HAL_GPIO_WritePin(nrf->ncsGpio->port, nrf->ncsGpio->pin,
			set ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
