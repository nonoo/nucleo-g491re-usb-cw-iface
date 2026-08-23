#include "uart-console.h"
#include "brd/brd.h"

#if LOGGING_ENABLED

UART_HandleTypeDef uart_console_handletypedef = {
	.Instance = UART_CONSOLE_UART,
	.Init.BaudRate = UART_CONSOLE_BAUDRATE,
	.Init.Mode = UART_MODE_TX_RX,
	.Init.Parity = UART_PARITY_NONE,
	.Init.WordLength = UART_WORDLENGTH_8B,
	.Init.StopBits = UART_STOPBITS_1,
	.Init.HwFlowCtl = UART_HWCONTROL_NONE,
	.Init.OverSampling = UART_OVERSAMPLING_8,
};

// This gets called when writing to stdio (printf).
int __io_putchar(int ch) {
	HAL_UART_Transmit(&uart_console_handletypedef, (uint8_t *)&ch, 1, 1000);
	return ch;
}

void uart_console_log(const char *tag, const char *fmt_s, ...) {
	va_list args;
	va_start(args, fmt_s);
	if (tag)
		printf("%s: ", tag);
	vprintf(fmt_s, args);
	va_end(args);
}

void uart_console_init(void) {
	UART_CONSOLE_TX_GPIO_ENABLE();
	UART_CONSOLE_RX_GPIO_ENABLE();
	UART_CONSOLE_CLK_ENABLE();

	GPIO_InitTypeDef gpio_init_structure = {
		.Pin = UART_CONSOLE_TX_PIN,
		.Mode = GPIO_MODE_AF_PP,
		.Speed = GPIO_SPEED_FREQ_HIGH,
		.Pull = GPIO_PULLUP,
		.Alternate = UART_CONSOLE_TX_AF,
	};
	HAL_GPIO_Init(UART_CONSOLE_TX_GPIO_PORT, &gpio_init_structure);

	gpio_init_structure.Pin = UART_CONSOLE_RX_PIN;
	gpio_init_structure.Mode = GPIO_MODE_AF_PP;
	gpio_init_structure.Alternate = UART_CONSOLE_RX_AF;
	HAL_GPIO_Init(UART_CONSOLE_RX_GPIO_PORT, &gpio_init_structure);

	if (HAL_UART_Init(&uart_console_handletypedef) != HAL_OK)
		brd_error_handler();
}

#endif
