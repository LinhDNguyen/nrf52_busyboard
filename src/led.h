#ifndef __LED_H__
#define __LED_H__ 1

typedef enum {
	LED_PIN_1 = 9,
	LED_PIN_2 = 10,
	LED_PIN_3,
	LED_PIN_4,
	LED_PIN_5,
	LED_PIN_6,
	LED_PIN_7,
	LED_PIN_8,
	LED_PIN_9,
	LED_PIN_10,
} led_pin_t;

typedef enum {
	LED_1 = 1 << LED_PIN_1,
	LED_2 = 1 << LED_PIN_2,
	LED_3 = 1 << LED_PIN_3,
	LED_4 = 1 << LED_PIN_4,
	LED_5 = 1 << LED_PIN_5,
	LED_6 = 1 << LED_PIN_6,
	LED_7 = 1 << LED_PIN_7,
	LED_8 = 1 << LED_PIN_8,
	LED_9 = 1 << LED_PIN_9,
	LED_10 = 1 << LED_PIN_10,
} led_t;

void led_init();
void led_set(u32_t leds);
void led_clear(u32_t leds);
void led_display(u32_t num);
void led_write(u32_t mask);

#endif /* __LED_H__ */