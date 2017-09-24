#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>
#include <logging/sys_log.h>

#include "led_effect.h"
#include "led.h"

#define STACK_SIZE 128

typedef void (*effect_handler_t)(void);

static struct k_thread s_led_thread;
static K_THREAD_STACK_DEFINE(s_stack, STACK_SIZE);
static u32_t s_period = 100; // 100ms is default
static char s_target = 0;
static char s_current = 0;
static u32_t s_loop_cnt = 0;
static effect_mode_t s_mode = EFFECT_MODE_NORMAL;
static char s_is_stop = 0;

static void s_eff_normal_handler(void);
static void s_eff_blink_handler(void);
static void s_eff_blink2_handler(void);
static void s_eff_free_handler(void);

static effect_handler_t handler_table[] = {
	s_eff_normal_handler,
	s_eff_blink_handler,
	s_eff_blink2_handler,
	s_eff_free_handler,
	NULL,
};

static void s_led_task_handler(void *id, void *unused1, void *unused2);
static void s_eff_reset(void);

static void s_led_task_handler(void *id, void *unused1, void *unused2)
{

	SYS_LOG_INF("effect task started");

	while(!s_is_stop) {
		k_sleep(s_period);

		effect_handler_t f = handler_table[s_mode];
		if (f) {
			f();
			s_loop_cnt++;
		}
	}

	SYS_LOG_INF("effect task stopped");
}

void effect_init(u32_t period_ms)
{
	s_period = period_ms;
	s_target = 0;
	s_current = 0;
	s_loop_cnt = 0;
	s_mode = EFFECT_MODE_NORMAL;
	s_is_stop = 0;

	led_init();

	led_display(0);

	// Start thread
	k_thread_create(&s_led_thread, s_stack, K_THREAD_STACK_SIZEOF(s_stack),
				s_led_task_handler, NULL, NULL, NULL, 4, 0, K_NO_WAIT);
	SYS_LOG_DBG("effect init done");
}

void effect_stop()
{
	s_is_stop = 1;
	SYS_LOG_DBG("effect stop");
}

void effect_set_period(u32_t period_ms)
{
	s_period = period_ms;
	SYS_LOG_DBG("new period %ums", s_period);
}

void effect_set_mode(effect_mode_t mode)
{
	s_mode = mode;
	SYS_LOG_DBG("new mode %d", s_mode);
	s_eff_reset();
}

void effect_next_mode(void)
{
	effect_mode_t mode = s_mode + 1;
	if (mode >= EFFECT_MODE_MAX) {
		mode = EFFECT_MODE_NORMAL;
	}
	s_mode = mode;
	SYS_LOG_DBG("next mode to %d", s_mode);
	s_eff_reset();
}

void effect_set_target(char ch)
{
	s_target = ch;
	SYS_LOG_DBG("new target %d-%c", s_target, s_target);
}

static void s_eff_reset(void)
{
	s_current = 0;
	s_loop_cnt = 0;
	SYS_LOG_DBG("effect reset");
	led_display(0);
}

static void s_eff_normal_handler(void)
{
	if (s_target == s_current) return;

	led_display(s_target);
	s_current = s_target;
}

static void s_eff_blink_handler(void)
{
	if (s_target == 0) return;

	if ((s_loop_cnt % 5) == 0) {
		if (s_current == s_target) {
			s_current = 0;
		} else {
			s_current = s_target;
		}
		SYS_LOG_DBG("loop count %d, current %d, target %d", s_loop_cnt, s_current, s_target);
	} else {
		return;
	}

	led_display(s_current);
}

static void s_eff_free_handler(void)
{
	u32_t i = s_loop_cnt % 10;

	// if (s_loop_cnt & 0x01) return;

	switch (i) {
		case 0:
		led_write(LED_1 | LED_3);
		break;

		case 1:
		led_write(LED_3 | LED_5);
		break;

		case 2:
		led_write(LED_5 | LED_7);
		break;

		case 3:
		led_write(LED_7 | LED_9);
		break;

		case 4:
		led_write(LED_9 | LED_10);
		break;

		case 5:
		led_write(LED_10 | LED_8);
		break;

		case 6:
		led_write(LED_8 | LED_6);
		break;

		case 7:
		led_write(LED_6 | LED_4);
		break;

		case 8:
		led_write(LED_4 | LED_2);
		break;

		case 9:
		led_write(LED_2 | LED_1);
		break;

		default:
		led_write(0);
		break;
	}
}

// Count from 0 -> target in loop
static void s_eff_blink2_handler(void) {
	static u32_t _s_return_cnt = 0;

	if (s_target == 0) return;

	if ((_s_return_cnt > 0) && (_s_return_cnt <= s_loop_cnt)) {
		s_current = 0;
		led_display(0);
		_s_return_cnt = 0;
	}

	if((_s_return_cnt == 0) && (s_loop_cnt & 0x01)) {

		led_display(s_current++);

		if (s_current > s_target) {
			s_current = 0;
			_s_return_cnt = s_loop_cnt + 10;
		} else {
			_s_return_cnt = 0;
		}
	}
}
