cmake_minimum_required(VERSION 3.20)

project(nucleo-g491re-usb-cw-iface C ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(FETCHCONTENT_QUIET FALSE)

include(FetchContent)
FetchContent_Declare(
    toolchain
    URL https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/15.3.rel1/arm-gnu-toolchain-15.3.rel1-x86_64-arm-none-eabi.tar.xz
    URL_HASH SHA256=563bebb2b97d53382b956d6ee1fe61e2cae26699901417234a37df505ef9b5fa
    DOWNLOAD_EXTRACT_TIMESTAMP OLD
)

FetchContent_Declare(
    tinyusb
    GIT_REPOSITORY https://github.com/hathach/tinyusb.git
    GIT_TAG 0.17.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(toolchain tinyusb)

set(CMAKE_C_COMPILER ${toolchain_SOURCE_DIR}/bin/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${toolchain_SOURCE_DIR}/bin/arm-none-eabi-gcc)
set(CMAKE_OBJCOPY ${toolchain_SOURCE_DIR}/bin/arm-none-eabi-objcopy)
set(CMAKE_SIZE ${toolchain_SOURCE_DIR}/bin/arm-none-eabi-size)

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsigned-char")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include src/config.h")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include src/main.h")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include src/uart/log.h")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCFG_TUSB_MCU=OPT_MCU_STM32G4")

set(CMAKE_C_FLAGS_DEBUG "-Og -g")
set(CMAKE_C_FLAGS_RELEASE "-Os -flto")

set(CMAKE_ASM_FLAGS "-Wall")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Werror")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=cortex-m4")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mthumb")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mfloat-abi=hard")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mfpu=fpv4-sp-d16")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -fmessage-length=0")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -fsigned-char")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -ffunction-sections")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -fdata-sections")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -include src/config.h")

set(CMAKE_ASM_FLAGS_DEBUG "-Og -g")
set(CMAKE_ASM_FLAGS_RELEASE "-Os")

set(CMAKE_EXE_LINKER_FLAGS "-T../STM32G491RETX_FLASH.ld --specs=../nano.specs --specs=nosys.specs -Xlinker --gc-sections -Wl,-Map,${PROJECT_NAME}.map")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME}.elf
	${SRC_FILES}
	src/brd/startup_stm32g491retx.s

	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c
	drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c
	drivers/BSP/STM32G4xx_Nucleo/stm32g4xx_nucleo.c
	drivers/CMSIS/Device/ST/STM32G4xx/Source/Templates/system_stm32g4xx.c

	${tinyusb_SOURCE_DIR}/src/tusb.c
	${tinyusb_SOURCE_DIR}/src/common/tusb_fifo.c
	${tinyusb_SOURCE_DIR}/src/device/usbd.c
	${tinyusb_SOURCE_DIR}/src/device/usbd_control.c
	${tinyusb_SOURCE_DIR}/src/class/hid/hid_device.c
	${tinyusb_SOURCE_DIR}/src/class/midi/midi_device.c
	${tinyusb_SOURCE_DIR}/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c
)

target_link_libraries(${PROJECT_NAME}.elf
	${CMAKE_SOURCE_DIR}/drivers/CMSIS/DSP/Lib/GCC/libarm_cortexM4lf_math.a
)

target_include_directories(${PROJECT_NAME}.elf PRIVATE
	${CMAKE_SOURCE_DIR}
	src
	src/brd
	src/usb
	${tinyusb_SOURCE_DIR}/src
	drivers/BSP/STM32G4xx_Nucleo
	drivers/CMSIS/Core/Include
	drivers/CMSIS/DSP/Include
	drivers/CMSIS/Device/ST/STM32G4xx/Include
	drivers/STM32G4xx_HAL_Driver/Inc
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}.elf>)

add_custom_target(size
    COMMAND ${CMAKE_SIZE} --format=berkeley ${PROJECT_NAME}.elf
    DEPENDS ${PROJECT_NAME}.elf
)

add_custom_target(${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    DEPENDS ${PROJECT_NAME}.elf
)
