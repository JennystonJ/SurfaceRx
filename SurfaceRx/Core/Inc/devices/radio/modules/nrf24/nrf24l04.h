/*
 * nrf24l04.h
 *
 *  Created on: Mar 7, 2022
 *      Author: Jennyston
 */

#ifndef INC_NRF24L04_H_
#define INC_NRF24L04_H_

#include <stdint.h>
#include <stdbool.h>
#include <stm32f3xx.h>
#include "devices/radio/modules/nrf24/nrf24l01_reg.h"
#include "peripherals/gpioPin.h"

#define MAX_PACKET_LENGTH 32

typedef struct {
	SPI_HandleTypeDef *hspi;
	GPIO_t *ncsGpio;
	GPIO_t *ceGpio;
} Nrf_t;

/**
  * @brief  Initialize NRF24L04 module
  * @param  nrf pointer to Nrf_t structure
  * @param	hspi pointer to SPI handle connected to NRF24L04 module
  * @param	ncs pointer to GPIO structure for NCS pin connected to NRF24L04
  * @param	ce pointer to GPIO structure for CE pin connected to NRF24L04
  * @retval None
  **/
void InitNrf(Nrf_t *nrf, SPI_HandleTypeDef *hspi, GPIO_t *ncs,
		GPIO_t *ce);

/**
  * @brief  Start NRF primary RX
  * @param  nrf pointer to Nrf_t structure
  * @retval None
  **/
void StartNrfPRX(Nrf_t *nrf);

/**
  * @brief  Stop NRF primary RX
  * @param  nrf pointer to Nrf_t structure
  * @retval None
  **/
void StopNrfPRX(Nrf_t *nrf);

/**
  * @brief  Transmit packet (NRF payload)
  * @param  nrf pointer to Nrf_t structure
  * @retval None
  **/
void TransmitNrfPacket(Nrf_t *nrf);

/**
  * @brief  return true if data is read, otherwise false
  * @param  nrf pointer to Nrf_t structure
  * @retval data ready state
  **/
bool IsNrfDataReady(Nrf_t *nrf);

/**
  * @brief  return true if data is sent, otherwise false
  * @param  nrf pointer to Nrf_t structure
  * @retval data sent state
  **/
bool IsNrfDataSent(Nrf_t *nrf);

/**
  * @brief  Read NRF register at provided address
  * @param  nrf pointer to Nrf_t structure
  * @param	reg register to be read
  * @param	size number of bytes to read at specified register
  * @param	data address of where read data is to be stored
  * @retval None
  **/
void ReadNrfRegister(Nrf_t *nrf, NrfReg reg, uint8_t size,
        uint8_t *data);

/**
  * @brief  Write NRF register at provided address
  * @param  nrf pointer to Nrf_t structure
  * @param	reg register to to be written to
  * @param	size number of bytes to written at specified register
  * @param	data address containing data to be written from
  * @retval None
  **/
void WriteNrfRegister(Nrf_t *nrf, NrfReg reg, uint8_t size,
        uint8_t *data);

/**
  * @brief  Read NRF RX payload
  * @param  nrf pointer to Nrf_t structure
  * @param	data address of where read payload is to be stored
  * @param	size number of payload bytes to read
  * @retval None
  **/
void ReadNrfRxPayload(Nrf_t *nrf, uint8_t *data, uint8_t size);

/**
  * @brief  Write NRF TX payload
  * @param  nrf pointer to Nrf_t structure
  * @param	data address of where payload is to be written from
  * @param	size number of payload bytes to write
  * @retval None
  **/
void WriteNrfTxPayload(Nrf_t *nrf, uint8_t *data, uint8_t size);

/**
  * @brief  Write NRF acknowledge payload
  * @param  nrf pointer to Nrf_t structure
  * @param	data address of where payload is to be written from
  * @param	size number of payload bytes to write
  * @retval None
  **/
void WriteNrfAckPayload(Nrf_t *nrf, uint8_t pipe, uint8_t *data,
		uint8_t size);

/**
  * @brief  Flush NRF TX FIFO
  * @param  nrf pointer to Nrf_t structure
  * @retval None
  **/
void FlushNrfTx(Nrf_t *nrf);

/**
  * @brief  Flush NRF RX FIFO
  * @param  nrf pointer to Nrf_t structure
  * @retval None
  **/
void FlushNrfRx(Nrf_t *nrf);

/**
  * @brief  Re-transmit previous payload
  * @param  nrf pointer to Nrf_t structure
  * @retval None
  **/
void ReuseNrfTxPayload(Nrf_t *nrf);

/**
  * @brief  Get width of pay load
  * @param  nrf pointer to Nrf_t structure
  * @retval None
  **/
uint8_t ReadNrfRxPayloadWidth(Nrf_t *nrf);

void WriteNrfTxPayloadNoAck(Nrf_t *nrf, uint8_t *data, uint8_t size);

uint8_t NopNrf(Nrf_t *nrf);







#endif /* INC_NRF24L04_H_ */
