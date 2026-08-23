#include "gpio.h"

void btn_handle_irq(void);

bool gpio_is_dit_active(void) {
	return HAL_GPIO_ReadPin(DIT_GPIO_PORT, DIT_PIN) == GPIO_PIN_RESET;
}

bool gpio_is_dah_active(void) {
	return HAL_GPIO_ReadPin(DAH_GPIO_PORT, DAH_PIN) == GPIO_PIN_RESET;
}

bool gpio_is_straight_key_active(void) {
	return (BSP_PB_GetState(BUTTON_USER) == 1) ||
		(HAL_GPIO_ReadPin(STRAIGHT_KEY_GPIO_PORT, STRAIGHT_KEY_PIN) == GPIO_PIN_RESET);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == DIT_PIN || GPIO_Pin == DAH_PIN || GPIO_Pin == STRAIGHT_KEY_PIN)
		btn_handle_irq();
}

void BSP_PB_Callback(Button_TypeDef Button) {
	if (Button == BUTTON_USER)
		btn_handle_irq();
}

void gpio_led_on(void) {
	BSP_LED_On(LED_GREEN);
}

void gpio_led_off(void) {
	BSP_LED_Off(LED_GREEN);
}

void gpio_init(void) {
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

	// Configuring the user button for both rising and falling edges with pull-down resistor to detect both press and release events.
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = USER_BUTTON_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(USER_BUTTON_GPIO_PORT, &GPIO_InitStruct);

	DIT_GPIO_ENABLE();
	DAH_GPIO_ENABLE();
	STRAIGHT_KEY_GPIO_ENABLE();

	GPIO_InitStruct.Pin = DIT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DIT_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DAH_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DAH_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = STRAIGHT_KEY_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(STRAIGHT_KEY_GPIO_PORT, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(DIT_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DIT_IRQn);

	HAL_NVIC_SetPriority(DAH_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DAH_IRQn);

	HAL_NVIC_SetPriority(STRAIGHT_KEY_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(STRAIGHT_KEY_IRQn);

	BSP_LED_Init(LED_GREEN);
}
