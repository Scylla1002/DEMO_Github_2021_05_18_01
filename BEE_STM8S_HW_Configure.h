#ifndef		_BEE_STM8S_HW_CONFIGURE_
#define		_BEE_STM8S_HW_CONFIGURE_


//#define     LED_BUILT_IN        PD_ODR_ODR3       //Kit 1K
#define     LED_BUILT_IN        PD_ODR_ODR0         //kit Discovery

#define		GATE1_PIN			PD_ODR_ODR5
#define		GATE1_DDR			PD_DDR_DDR5
#define		GATE1_CR1			PD_CR1_C15
#define		GATE1_CR2			PD_CR2_C25


#define		GATE2_PIN			PD_ODR_ODR6
#define		GATE2_DDR			PD_DDR_DDR6
#define		GATE2_CR1			PD_CR1_C16
#define		GATE2_CR2			PD_CR2_C26

void GPIO_Configure();
void Clock_Configure();
void eeprom_unlock();

#endif

