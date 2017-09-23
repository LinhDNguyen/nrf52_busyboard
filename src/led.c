#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>

#include "led.h"

#define GPIO_NAME	CONFIG_GPIO_NRF5_P0_DEV_NAME

static struct device *gpiob = NULL;

void led_init()
{
	int i;

	gpiob = device_get_binding(GPIO_NAME);
	if (!gpiob) {
		printk("error\n");
		return;
	}

	// Configure leds
	for (i = LED_PIN_1; i <= LED_PIN_10; ++i) {
		gpio_pin_configure(gpiob, i, GPIO_DIR_OUT);
	}
}

void led_set(u32_t leds)
{
	int i;

	for (i = LED_PIN_1; i <= LED_PIN_10; ++i) {
		if (leds & (1 << i)) {
			// set
			gpio_pin_write(gpiob, i, 0);
		}
	}
}

void led_clear(u32_t leds)
{
	int i;

	for (i = LED_PIN_1; i <= LED_PIN_10; ++i) {
		if (leds & (1 << i)) {
			// clear
			gpio_pin_write(gpiob, i, 1);
		}
	}
}

void led_write(u32_t mask)
{
	int i;

	for (i = LED_PIN_1; i <= LED_PIN_10; ++i) {
		printk("==%d", i);
		if (mask & (1 << i)) {
			// set
			printk(":1\n");
			gpio_pin_write(gpiob, i, 0);
		} else {
			// clear
			printk(":0\n");
			gpio_pin_write(gpiob, i, 1);
		}
	}
}

void led_display(u32_t num)
{
	printk("led_display: %u\n", num);
	switch (num) {
		case 1:
		led_write(LED_1);
		break;

		case 2:
		led_write(LED_1 | LED_2);
		break;

		case 3:
		led_write(LED_1 | LED_2 | LED_3);
		break;

		case 4:
		led_write(LED_1 | LED_2 | LED_3 | LED_4);
		break;

		case 5:
		led_write(LED_1 | LED_2 | LED_3 | LED_4 | LED_5);
		break;

		case 6:
		led_write(LED_1 | LED_2 | LED_3 | LED_4 | LED_5 | LED_6);
		break;

		case 7:
		led_write(LED_1 | LED_2 | LED_3 | LED_4 | LED_5 | LED_6 | LED_7);
		break;

		case 8:
		led_write(LED_1 | LED_2 | LED_3 | LED_4 | LED_5 | LED_6 | LED_7 | LED_8);
		break;

		case 9:
		led_write(LED_1 | LED_2 | LED_3 | LED_4 | LED_5 | LED_6 | LED_7 | LED_8 | LED_9);
		break;

		case 10:
		led_write(LED_1 | LED_2 | LED_3 | LED_4 | LED_5 | LED_6 | LED_7 | LED_8 | LED_9 | LED_10);
		break;

		default:
		break;
	}
}
