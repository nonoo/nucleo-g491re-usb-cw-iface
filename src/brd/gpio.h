#ifndef BRD_GPIO_H_
#define BRD_GPIO_H_

bool gpio_is_dit_active(void);
bool gpio_is_dah_active(void);
bool gpio_is_straight_key_active(void);

void gpio_led_on(void);
void gpio_led_off(void);

void gpio_init(void);

#endif /* BRD_GPIO_H_ */
