#include "stm32f10x.h"                  // Device header


#define HIGH 1
#define LOW 0

void delay(__IO uint32_t timedelay){ 
	for(int i=0; i<timedelay; i++){}
}



void GPIO_Config(void) {
RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //Kich hoat xung clock cap cho GPIOC
											
GPIOC->CRH |= GPIO_CRH_MODE13_0; 	//MODE13[1:0] = 11: Output mode, max speed 50 MHz
GPIOC->CRH |= GPIO_CRH_MODE13_1; 	
GPIOC->CRH &= ~GPIO_CRH_CNF13_0;	              //CNF13[1:0] = 00: General purpose output push-pull
GPIOC->CRH &= ~GPIO_CRH_CNF13_1;


}

int main() {
	GPIO_Config();
	// Configure RCC and GPIO once
	while(1){
	GPIOC->ODR |= 1<<13; // Ghi 1 ra PC13
	delay(10000000);
	GPIOC->ODR &= ~(1<<13); // Ghi 0 ra PC13
	delay(10000000);
}

}