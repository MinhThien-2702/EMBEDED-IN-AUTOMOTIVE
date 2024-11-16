#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

#define ADDR_STR_BLINK 0x08001000       // Define the start address for your firmware or bootloader

int main() {

    RCC_DeInit();


    SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);


    __set_MSP(*(__IO uint32_t*)(ADDR_STR_BLINK));


    uint32_t JumpAddress = *(__IO uint32_t*)(ADDR_STR_BLINK + 4);


    void (*reset_handler)(void) = (void (*)(void))JumpAddress;


    reset_handler();

    while (1) {

    }
}
