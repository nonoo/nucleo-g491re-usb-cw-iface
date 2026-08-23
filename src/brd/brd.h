#ifndef BRD_H_
#define BRD_H_

#define BRD_IS_INTERRUPT() (__get_IPSR() != 0)

void brd_error_handler(void);

uint32_t brd_tick_diff(uint32_t start, uint32_t end);
uint32_t brd_get_tick(void);

void brd_enter_sleep(void);

#endif // BRD_H_
