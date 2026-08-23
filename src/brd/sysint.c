#include "brd.h"
#include "tusb.h"

void NMI_Handler(void) {
	while (1)
		;
}

void HardFault_Handler(void) {
	while (1)
		;
}

void MemManage_Handler(void) {
	while (1)
		;
}

void BusFault_Handler(void) {
	while (1)
		;
}

void UsageFault_Handler(void) {
	while (1)
		;
}

void SVC_Handler(void) {
}

void DebugMon_Handler(void) {
}

void PendSV_Handler(void) {
}

void SysTick_Handler(void) {
	HAL_IncTick();
}

void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(DIT_PIN);
}

void EXTI1_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(DAH_PIN);
}

void EXTI15_10_IRQHandler(void) {
	BSP_PB_IRQHandler(BUTTON_USER);
}

void LPUART1_IRQHandler(void) {
	extern UART_HandleTypeDef uart_console_handletypedef;
	HAL_UART_IRQHandler(&uart_console_handletypedef);
}

void USB_HP_IRQHandler(void) {
	tud_int_handler(0);
}

void USB_LP_IRQHandler(void) {
	tud_int_handler(0);
}

void USBWakeUP_IRQHandler(void) {
	tud_int_handler(0);
}

#if DEBUG /* Setting dummy interrupt handlers */
void WWDG_IRQHandler(void) {
	brd_error_handler();
}
void PVD_PVM_IRQHandler(void) {
	brd_error_handler();

}
void RTC_TAMP_CSS_LSE_IRQHandler(void) {
	brd_error_handler();
}
void RTC_WKUP_IRQHandler(void) {
	brd_error_handler();
}
void FLASH_IRQHandler(void) {
	brd_error_handler();
}
void RCC_IRQHandler(void) {
	brd_error_handler();
}
void EXTI2_IRQHandler(void) {
	brd_error_handler();
}
void EXTI3_IRQHandler(void) {
	brd_error_handler();
}
void EXTI4_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(STRAIGHT_KEY_PIN);
}
void DMA1_CH3_IRQHandler(void) {
	brd_error_handler();
}
void DMA1_CH4_IRQHandler(void) {
	brd_error_handler();
}
void DMA1_CH5_IRQHandler(void) {
	brd_error_handler();
}
void DMA1_CH6_IRQHandler(void) {
	brd_error_handler();
}
void DMA1_CH7_IRQHandler(void) {
	brd_error_handler();
}
void ADC1_2_IRQHandler(void) {
	brd_error_handler();
}
void FDCAN1_IT0_IRQHandler(void) {
	brd_error_handler();
}
void FDCAN1_IT1_IRQHandler(void) {
	brd_error_handler();
}
void EXTI9_5_IRQHandler(void) {
	brd_error_handler();
}
void TIM1_BRK_TIM15_IRQHandler(void) {
	brd_error_handler();
}
void TIM1_UP_TIM16_IRQHandler(void) {
	brd_error_handler();
}
void TIM1_TRG_COM_IRQHandler(void) {
	brd_error_handler();
}
void TIM1_CC_IRQHandler(void) {
	brd_error_handler();
}
void TIM2_IRQHandler(void) {
	brd_error_handler();
}
void TIM3_IRQHandler(void) {
	brd_error_handler();
}
void TIM4_IRQHandler(void) {
	brd_error_handler();
}
void I2C1_EV_IRQHandler(void) {
	brd_error_handler();
}
void I2C1_ER_IRQHandler(void) {
	brd_error_handler();
}
void I2C2_EV_IRQHandler(void) {
	brd_error_handler();
}
void I2C2_ER_IRQHandler(void) {
	brd_error_handler();
}
void SPI1_IRQHandler(void) {
	brd_error_handler();
}
void SPI2_IRQHandler(void) {
	brd_error_handler();
}
void USART2_IRQHandler(void) {
	brd_error_handler();
}
void USART3_IRQHandler(void) {
	brd_error_handler();
}
void RTC_ALARM_IRQHandler(void) {
	brd_error_handler();
}
void TIM8_BRK_IRQHandler(void) {
	brd_error_handler();
}
void TIM8_UP_IRQHandler(void) {
	brd_error_handler();
}
void TIM8_TRG_COM_IRQHandler(void) {
	brd_error_handler();
}
void TIM8_CC_IRQHandler(void) {
	brd_error_handler();
}
void ADC3_IRQHandler(void) {
	brd_error_handler();
}
void LPTIM1_IRQHandler(void) {
	brd_error_handler();
}
void SPI3_IRQHandler(void) {
	brd_error_handler();
}
void UART4_IRQHandler(void) {
	brd_error_handler();
}
void UART5_IRQHandler(void) {
	brd_error_handler();
}
void TIM6_DACUNDER_IRQHandler(void) {
	brd_error_handler();
}
void TIM7_IRQHandler(void) {
	brd_error_handler();
}
void DMA2_CH1_IRQHandler(void) {
	brd_error_handler();
}
void DMA2_CH2_IRQHandler(void) {
	brd_error_handler();
}
void DMA2_CH3_IRQHandler(void) {
	brd_error_handler();
}
void DMA2_CH4_IRQHandler(void) {
	brd_error_handler();
}
void DMA2_CH5_IRQHandler(void) {
	brd_error_handler();
}
void UCPD1_IRQHandler(void) {
	brd_error_handler();
}
void COMP1_2_3_IRQHandler(void) {
	brd_error_handler();
}
void COMP4_5_6_IRQHandler(void) {
	brd_error_handler();
}
void COMP7_IRQHandler(void) {
	brd_error_handler();
}
void CRS_IRQHandler(void) {
	brd_error_handler();
}
void SAI_IRQHandler(void) {
	brd_error_handler();
}
void TIM20_BRK_IRQHandler(void) {
	brd_error_handler();
}
void TIM20_UP_IRQHandler(void) {
	brd_error_handler();
}
void TIM20_TRG_COM_IRQHandler(void) {
	brd_error_handler();
}
void TIM20_CC_IRQHandler(void) {
	brd_error_handler();
}
void FPU_IRQHandler(void) {
	brd_error_handler();
}
void RNG_IRQHandler(void) {
	brd_error_handler();
}
void LPUART_IRQHandler(void) {
	brd_error_handler();
}
void I2C3_EV_IRQHandler(void) {
	brd_error_handler();
}
void I2C3_ER_IRQHandler(void) {
	brd_error_handler();
}
void DMAMUX_OVR_IRQHandler(void) {
	brd_error_handler();
}
void DMA1_CH8_IRQHandler(void) {
	brd_error_handler();
}
void DMA2_CH6_IRQHandler(void) {
	brd_error_handler();
}
void DMA2_CH7_IRQHandler(void) {
	brd_error_handler();
}
void DMA2_CH8_IRQHandler(void) {
	brd_error_handler();
}
void Cordic_IRQHandler(void) {
	brd_error_handler();
}
void FMAC_IRQHandler(void) {
	brd_error_handler();
}
#endif
