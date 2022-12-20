/*
 * servo.c
 *
 *  Created on: Dec 3, 2022
 *      Author: Jennyston
 */

#include "devices/servo.h"
#include "utilities/map.h"
#include "assert.h"


void InitServo(Servo_t *servo, TIM_HandleTypeDef *htim, uint32_t timChannel,
		uint16_t position) {
	ASSERT(servo != 0);
	ASSERT(htim != 0);
	ASSERT(position <= SERVO_POSITION_MAX);

	//initialize parameters
	servo->htim = htim;
	servo->timChannel = timChannel;
	SetServoPosition(servo, position);
}

void SetServoPosition(Servo_t *servo, uint16_t position) {
	ASSERT(position <= SERVO_POSITION_MAX);

	//convert position to value within 1-2ms PWM pulse range
	uint32_t pwmMappedPos = map(position, SERVO_POSITION_MIN,
			SERVO_POSITION_MAX, SERVO_MIN_PULSE, SERVO_MAX_PULSE);

	//modify PWM pulse width
	__HAL_TIM_SET_COMPARE(servo->htim, servo->timChannel, pwmMappedPos);

	//update stored servo position
	servo->position = position;
}
