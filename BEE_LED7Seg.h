#ifndef		_BEE_LED7SEG_H_
#define		_BEE_LED7SEG_H_

#define		LED1_PORT		PA_ODR
#define		LED2_PORT		PB_ODR
#define		LED3_PORT		PC_ODR
#define		LED4_PORT		PD_ODR

#define		SegA_BIT		7
#define		SegB_BIT		6
#define		SegC_BIT		5
#define		SegD_BIT		4
#define		SegE_BIT		3
#define		SegF_BIT		2
#define		SegG_BIT		1
#define		SegH_BIT		0		

#define		Code_Num_Base_0			0xC0
#define		Code_Num_Base_1			0xF9
#define		Code_Num_Base_2			0xA4
#define		Code_Num_Base_3			0xB0
#define		Code_Num_Base_4			0x99
#define		Code_Num_Base_5			0x92
#define		Code_Num_Base_6			0x82
#define		Code_Num_Base_7			0xF8
#define		Code_Num_Base_8			0x80
#define		Code_Num_Base_9			0x90

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

static unsigned char LED_Table_PC[10] = {0x11, 0x7D, 0x23, 0x29, 0x4D, 0x89, 0x81, 0x3D, 0x01, 0x09};
static unsigned char LED_Table_PB[10] = {0x05, 0x5F, 0x23, 0x0B, 0x59, 0x89, 0x81, 0x1F, 0x01, 0x09};

#define		PC_Mask					0xF8
#define		PB_Mask					0x30

//#define		LED7Seg_Display(x)		PC_ODR |= PC_Mask				        \
//										PC_ODR &= (LED_Table_PC[x]	| 0x07)	    \
//										PB_ODR |= PB_Mask				        \
//										PB_ODR &= (LED_Table_PB[x] | 0xCF)		\

void LED7Seg_Configure();
void LED7Seg_Display(unsigned char number);
#endif

