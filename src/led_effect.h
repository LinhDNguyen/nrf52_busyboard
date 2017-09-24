#ifndef __LED_EFFECT_H__
#define __LED_EFFECT_H__ 1

typedef enum {
	EFFECT_MODE_NORMAL = 0,
	EFFECT_MODE_BLINK,
	EFFECT_MODE_STRIP,
	EFFECT_MODE_FREE,
	EFFECT_MODE_MAX,
} effect_mode_t;

void effect_init(u32_t period_ms);
void effect_stop();
void effect_set_period(u32_t period_ms);
void effect_set_mode(effect_mode_t mode);
void effect_set_target(char ch);
void effect_next_mode(void);

#endif /* __LED_EFFECT_H__ */