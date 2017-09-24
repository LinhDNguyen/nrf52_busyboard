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
static char s_loop_cnt = 0;
static effect_mode_t s_mode = EFFECT_MODE_NORMAL;
static char s_is_stop = 0;

static void s_eff_normal_handler(void);
static void s_eff_blink_handler(void);
static void s_eff_strip_handler(void);
static void s_eff_free_handler(void);

static effect_handler_t handler_table[] = {
	s_eff_normal_handler,
	s_eff_blink_handler,
	s_eff_strip_handler,
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

static void s_eff_normal_handler(void)
{
	if (s_target == s_current) return;

	led_display(s_target);
	s_current = s_target;
}

static void s_eff_blink_handler(void)
{

}

static void s_eff_strip_handler(void)
{

}

static void s_eff_free_handler(void)
{

}

static void s_eff_reset(void)
{
	s_current = 0;
	SYS_LOG_DBG("effect reset");
	led_display(0);
}
