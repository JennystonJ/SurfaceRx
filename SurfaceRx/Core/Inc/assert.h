/*
 * assert.h
 *
 *  Created on: Dec 4, 2022
 *      Author: Jennyston
 */

#ifndef INC_ASSERT_H_
#define INC_ASSERT_H_

//define USE_ASSERT to enable assertions
#define USE_ASSERT

#ifdef USE_ASSERT
#define ASSERT(expr) \
	if(expr) \
		{} \
	else \
		aFailed(__FILE__, __LINE__)
#else
#define ASSERT(ignore) ((void)0U)
#endif


/**
  * @brief  Outputs failed assertion to UART port
  * @param  file string with file name of failed assertion
  * @param	line string with line number of failed assertion
  * @retval None
  **/
void aFailed(const char *file, int line);

#endif /* INC_ASSERT_H_ */
