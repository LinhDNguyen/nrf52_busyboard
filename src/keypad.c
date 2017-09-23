#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>

#include "keypad.h"

#define GPIO_NAME	CONFIG_GPIO_NRF5_P0_DEV_NAME

const uint8_t row_pins[] = {30, 29, 28, 27};
const uint8_t col_pins[] = {26, 25, 24};
const char chars[][3] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'},
};

static struct device *gpiob = NULL;
static char s_prev_char = 0;

void keypad_config()
{
	int i;

	gpiob = device_get_binding(GPIO_NAME);
	if (!gpiob) {
		printk("error\n");
		return;
	}

	// Configure row pins
	for (i = 0; i < sizeof(row_pins); ++i) {
		gpio_pin_configure(gpiob, row_pins[i], GPIO_DIR_OUT);
	}

	// Configure col pins
	for (i = 0; i < sizeof(col_pins); ++i) {
		gpio_pin_configure(gpiob, col_pins[i], GPIO_DIR_IN |  GPIO_PUD_PULL_DOWN);
	}
}

char keypad_scan()
{
	int i,j;
	int ret = 0;
	u32_t val;

	for (i = 0; i < sizeof(row_pins); ++i) {
		ret = gpio_pin_write(gpiob, row_pins[i], 1);
		k_sleep(1);

		for (j = 0; j < sizeof(col_pins); ++j) {
			ret = gpio_pin_read(gpiob, col_pins[j], &val);
			if (val != 0) {
				// Filter if hold a key
				if (0 != s_prev_char) {
					// printk("prev: %c,nothing\n", s_prev_char);
					goto lb_nothing;
				}
				s_prev_char = chars[i][j];
				// printk("OK: %c\n", s_prev_char);
				return chars[i][j];
			}
		}

		ret = gpio_pin_write(gpiob, row_pins[i], 0);
	}

	// printk("NONE\n");
	s_prev_char = 0;
lb_nothing:
	return 0;
}
