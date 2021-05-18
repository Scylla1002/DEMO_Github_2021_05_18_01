#include  	"iostm8s003f3.h"
#include 	"BEE_LED7Seg.h"

											
/*
                                      			STM8S003F3P5 - RGB45A
                                      				____________
IR_Receiver     	UART1_CK/TIM2_CH1/BEEP/(HS)PD4|(01)			 (20)|PD3(HS)/AIN4/TIM2_CH2/ADC_ETR					Button_1
Sync_Input            		 UART1_TX/AIN5/(HS)PD5|(02)			 (19)|PD2(HS)/AIN3/[TIM2_CH3]						Button_2
ADC_Microphone                UART1_RX/AIN6(HS)PD6|(03)			 (18)|PD1(HS)/SWIM									Button_3
                               				  NRST|(04)			 (17)|PC7/(HS)/SPI_MISO[TIM1_CH2]					LED_7Seg_B
LED_RED                          		 OSCIN/PA1|(05)			 (16)|PC6(HS)/SPI_MOSI[TIM1_CH1]					LED_7Seg_A
LED_GREEN                         		OSCOUT/PA2|(06)			 (15)|PC5(HS)/SPI_SCK[TIM2_CH1]						LED_7Seg_F 
                                			   GND|(07)			 (14)|PC4(HS)/TIM1_CH4/CLK_CCO/AIN2/[TIM1_CH2N]		LED_7Seg_G
                               				  VCAP|(08)			 (13)|PC3(HS)/TIM1_CH3[TLI][TIM1_CH1N]				LED_7Seg_E
                                			   VDD|(09)			 (12)|(Open Drain)PB4(T)/I2C_SCL[ADC_ETR]			LED_7Seg_D
LED_BLUE          		 [SPI_NSS]TIM2_CH3/(HS)PA3|(10)__________(11)|(Open Drain)PB5(T)/I2C_SDA[TIM1_BKIN]			LED_7Seg_C
*/


static void GPIO_Configure(){
	//PA_DDR_DDR1 = 1;
//	PB_DDR = 0xFF;
}

void LED7Seg_Configure(){
	GPIO_Configure();
	
	
}

void LED7Seg_Display(unsigned char number){
	PC_ODR |= PC_Mask;
	PC_ODR &= LED_Table_PC[number] | 0x07;
	PB_ODR |= PB_Mask;
	PB_ODR &= LED_Table_PB[number] | 0xCF;
    
//#define		LED7Seg_Display(x)		PC_ODR |= PC_Mask				        \
//										PC_ODR &= (LED_Table_PC[x]	| 0x07)	    \
//										PB_ODR |= PB_Mask				        \
//										PB_ODR &= (LED_Table_PB[x] | 0xCF)		\    
}


