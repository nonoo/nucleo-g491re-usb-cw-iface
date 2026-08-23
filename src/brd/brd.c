#include "brd.h"

static __unused const char *TAG = "brd";

void brd_error_handler(void) {
#if DEBUG
	__disable_irq();
	while (1)
		;
#else
	NVIC_SystemReset();
#endif
}

#if DEBUG
void __assert_func(const char *file, int line, const char *func, const char *arg) {
	LOG(TAG, "assert %s:%u %s %s\n", file, line, func, arg);
	brd_error_handler();
	__builtin_unreachable();
}
#endif

uint32_t brd_tick_diff(uint32_t start, uint32_t end) {
	return (end >= start) ? (end - start) : (UINT32_MAX - start + end + 1);
}

uint32_t brd_get_tick(void) {
	return HAL_GetTick();
}

void brd_clk_init(void) {
	// 1. Voltage Scaling:
	// Configure the internal voltage regulator to Range 2 (~1.0V core voltage) instead
	// of the power-on reset default Range 1 (~1.28V). This lowers dynamic power (P ~ V^2 * f)
	// by ~39% across the core domain and decreases static leakage.
	// Range 2 supports system clock frequencies up to 26 MHz (our target is 16 MHz).
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);

	// 2. Oscillators:
	// Enable the 16 MHz internal RC oscillator (HSI16) as the system clock source.
	// Explicitly disable all unused oscillators (HSE, LSE, LSI, PLL) to eliminate
	// unnecessary oscillator biasing and quiescent current consumption.
	// (Note: HSI48 for USB is enabled on-demand in usb_init()).
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE |
		RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
	RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		brd_error_handler();

	// 3. System Clocks & Bus Prescalers:
	// Set SYSCLK to HSI16 (16 MHz) and AHB, APB1, APB2 prescalers to DIV1 (HCLK = PCLK1 = PCLK2 = 16 MHz).
	// 16 MHz is the bare minimum system/bus frequency that satisfies the hardware requirement
	// of f_PCLK1 >= 10 MHz for the STM32 USB Full-Speed device peripheral without running a PLL.
	// Flash read latency is set to 0 wait states (FLASH_LATENCY_0), suitable for 16 MHz in Range 2.
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
		RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
		brd_error_handler();

	// 4. Flash Power-Down in Sleep:
	// Enable FLASH_ACR_SLEEP_PD so the Flash memory enters deep power-down mode
	// during CPU Sleep (WFI), significantly reducing standby leakage during inactivity.
	__HAL_FLASH_SLEEP_POWERDOWN_ENABLE();
}

void brd_enter_sleep(void) {
	HAL_SuspendTick();
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	HAL_ResumeTick();
}
