/*
 * assert.c
 *
 *  Created on: Dec 4, 2022
 *      Author: Jennyston
 */

#include "assert.h"
#include <stdio.h>

void aFailed(const char *file, int line) {
	printf("%d: Error in %s", line, file);
}
