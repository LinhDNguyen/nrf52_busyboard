#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>
#include <logging/sys_log.h>

#include "led.h"

#define GPIO_NAME	CONFIG_GPIO_NRF5_P0_DEV_NAME
#define ON 0
#define OFF 1

static struct device *gpiob = NULL;
static u8_t s_led_pins[] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4, LED_PIN_5, LED_PIN_6, LED_PIN_7, LED_PIN_8, LED_PIN_9, LED_PIN_10};

void led_init()
{
	int i;

	gpiob = device_get_binding(GPIO_NAME);
	if (!gpiob) {
		SYS_LOG_ERR("Cannot get GPIO device %s", GPIO_NAME);
		return;
	}

	// Configure leds
	for (i = 0; i < sizeof(s_led_pins); ++i) {
		SYS_LOG_DBG("gpio %d/%d %d -> %d", i + 1, sizeof(s_led_pins), s_led_pins[i], OFF);
		gpio_pin_configure(gpiob, s_led_pins[i], GPIO_DIR_OUT);
		gpio_pin_write(gpiob, s_led_pins[i], OFF);
	}
	SYS_LOG_INF("%d leds configured", sizeof(s_led_pins));
}

void led_set(u32_t leds)
{
	int i;

	for (i = 0; i < sizeof(s_led_pins); ++i) {
		if (leds & (1 << s_led_pins[i])) {
			// set
			gpio_pin_write(gpiob, s_led_pins[i], ON);
		}
	}
}

void led_clear(u32_t leds)
{
	int i;

	for (i = 0; i < sizeof(s_led_pins); ++i) {
		if (leds & (1 << s_led_pins[i])) {
			// clear
			gpio_pin_write(gpiob, s_led_pins[i], OFF);
		}
	}
}

void led_write(u32_t mask)
{
	int i;

	for (i = 0; i < sizeof(s_led_pins); ++i) {
		// printk("==%d", i);
		if (mask & (1 << s_led_pins[i])) {
			// set
			// printk(":1\n");
			gpio_pin_write(gpiob, s_led_pins[i], ON);
		} else {
			// clear
			// printk(":0\n");
			gpio_pin_write(gpiob, s_led_pins[i], OFF);
		}
	}
}

void led_display(u32_t num)
{
	// printk("led_display: %u\n", num);
	switch (num) {
		case 0:
		led_write(0);
		break;

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
