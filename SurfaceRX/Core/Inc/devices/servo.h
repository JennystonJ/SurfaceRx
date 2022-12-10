/*
 * servo.h
 *
 *  Created on: Dec 3, 2022
 *      Author: Jennyston
 */

#ifndef INC_DEVICES_SERVO_H_
#define INC_DEVICES_SERVO_H_

#include <stdint.h>
#include <stm32f3xx_hal.h>

#define SERVO_PULSE_PERIOD_MS 20

#define SERVO_PULSE_MIN_TIME_MS 1
#define SERVO_PULSE_MAX_TIME_MS 2

#define SERVO_ARR_MAX (40000-1)
#define SERVO_ARR_PER_MS (SERVO_ARR_MAX/SERVO_PULSE_PERIOD_MS)

#define SERVO_MIN_PULSE (SERVO_ARR_PER_MS * SERVO_PULSE_MIN_TIME_MS)
#define SERVO_MAX_PULSE (SERVO_ARR_PER_MS * SERVO_PULSE_MAX_TIME_MS)

#define SERVO_PULSE_RANGE (SERVO_MAX_PULSE - SERVO_MIN_PULSE)

#define SERVO_POSITION_MIN 0
#define SERVO_POSITION_MAX 1023

typedef struct {
	TIM_HandleTypeDef *htim;
	uint32_t timChannel;
	uint16_t position;
} Servo_t;

/**
  * @brief  Initializes servo
  * @param  servo pointer to servo handle
  * @param	htim pointer to PWM timer handle
  * @param	timChannel PWM channel
  * @param	position desired servo position (10-bit)
  * @retval None
  **/
void InitServo(Servo_t *servo, TIM_HandleTypeDef *htim, uint32_t timChannel,
		uint16_t position);

/**
  * @brief  Sets servo position
  * @param  servo pointer to servo handle
  * @param	position desired servo position (10-bit)
  * @retval None
  **/
void SetServoPosition(Servo_t *servo, uint16_t position);

#endif /* INC_DEVICES_SERVO_H_ */
