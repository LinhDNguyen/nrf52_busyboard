/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>

#include "keypad.h"

void main(void)
{
	keypad_config();

	printk("Press KEYPAD\n");
	while (1) {
		k_sleep(100);
		char val = keypad_scan();

		if (val != 0) {
			// Has key pressed
			printk("+%c+\n", val);
		}
	}
}
