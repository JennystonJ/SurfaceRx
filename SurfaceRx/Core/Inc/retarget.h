/*
 * retarget.h
 *
 *  Created on: Apr 11, 2021
 *      Author: Jennyston
 */

#ifndef INC_RETARGET_H_
#define INC_RETARGET_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include "stm32f3xx_hal.h"
#include <sys/stat.h>

	void RetargetInit(UART_HandleTypeDef *huart);
	int _isatty(int fd);
	int _write(int fd, char* ptr, int len);
	int _close(int fd);
	int _lseek(int fd, int ptr, int dir);
	int _read(int fd, char* ptr, int len);
	int _fstat(int fd, struct stat* st);


#ifdef __cplusplus
	}
#endif

#endif /* INC_RETARGET_H_ */
