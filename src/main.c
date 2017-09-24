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
#include <logging/sys_log.h>

#include "keypad.h"
#include "led_effect.h"

void main(void)
{
	keypad_config();
	SYS_LOG_DBG("keypad init done");
	effect_init(100); // period 100ms
	SYS_LOG_DBG("led init done");

	SYS_LOG_INF("=========KEYPAD READY=========");
	while (1) {
		k_sleep(100);
		char val = keypad_scan();

		if (val != 0) {
			// Has key pressed
			SYS_LOG_DBG("%c pressed", val);
			if ((val >= '0') && (val <= '9')) {
				effect_set_target(val - '0');
			} else if (val == '#') { // TODO: change to '*'
				effect_set_target(10);
			} else {
				effect_next_mode();
			}
		}
	}
}
