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
