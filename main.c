#include  <iostm8s003f3.h>
//#include  <iostm8s105k6.h>
#include  <intrinsics.h>
#include  <string.h>
#include "BEE_LED7Seg.h"
#include "BEE_STM8S_HW_Configure.h"
#include "sound.h"



void delay_us(unsigned int time)
{
	while(time--)
	{
		__no_operation();
		__no_operation();
		__no_operation();
		__no_operation();
		__no_operation();
	}
}

void delay_ms(unsigned int time)
{
  	while(time--)
    	delay_us(1000);
}



/*------------------------------------------------------------------------
------------------------------------------------------------------------*/


/*------------------------------------------------------------------------
------------------------------------------------------------------------*/


/*------------------------------------------------------------------------
------------------------------------------------------------------------*/
void GPIO_Init(){
	PA_ODR_ODR1 = 1;
	PA_CR2_C21 = 0;
	PA_DDR_DDR1 = 0;
	PA_CR1_C11 = 1;

	PA_DDR_DDR2 = 1;
	PA_CR1_C12 = 1;
	PA_CR2_C22 = 1;

	PD_DDR_DDR3 = 1;
	PD_CR1_C13 = 1;
	PD_CR2_C23 = 1;
}

static void TIM_Init(){
	TIM4_ARR = 50 - 1;
	TIM4_PSCR = 0x04;
	TIM4_IER = 0x01;
	TIM4_CR1 = 0x81;
}

/*RF 433 Decode

Date Start: 2021-05-13
*/

#define		RF_IDR			PD_IDR_IDR5
#define		RF_DDR			PD_DDR_DDR5
#define		RF_CR1			PD_CR1_C15
#define		RF_CR2			PD_CR2_C25

void RF433_Configure(){
	
	RF_DDR = 0;
	RF_CR1 = 0;
	RF_CR2 = 0;
	
	TIM_Init();
}

struct {
  unsigned long Data;
  unsigned char BitCount;
  unsigned char Hold;
} RF = {0};

#pragma vector = TIM4_OVR_UIF_vector
__interrupt void TIM4_ISR() {
    TIM4_SR = 0x00;

    static unsigned long data = 0;
    static struct {
        unsigned char sum: 4;
        unsigned char logic: 1;
        unsigned char logic_old: 1;
    }
    rf_var = {
        0,
        1,
        1
    };
    static unsigned short timeCount = 0;
    static unsigned char bitCount = 0;
    static unsigned char timeH = 0;

    if (RF_IDR) {
        if (rf_var.sum < 5)
            rf_var.sum++;
    } else if (rf_var.sum)
        rf_var.sum--;

    if (rf_var.sum < 2)
        rf_var.logic = 0;
    else if (rf_var.sum >= 3)
        rf_var.logic = 1;
//    PA_ODR_ODR2 = rf_var.logic;

    if (rf_var.logic != rf_var.logic_old) {
        rf_var.logic_old = rf_var.logic;
        if (timeCount >= 50) {
            if (rf_var.logic) {
                if (bitCount == 24 || bitCount == 32 || bitCount == 33) {
                    if (RF.Data == 0) {
                        RF.Data = data;
                        RF.BitCount = bitCount;
                    }
                    RF.Hold = 1;
                }
            }
            bitCount = 0;
            data = 0;
        } else {
            if (!rf_var.logic)
                timeH = timeCount;
            else {
                data <<= 1;
                if (timeH > timeCount) {
                    data |= 1;
                }
                bitCount++;
            }
        }
        timeCount = 0;
    }
    if (timeCount < 1000)
        timeCount++;
    else
        RF.Hold = 0;
}

unsigned long key1 = 0x006E66A8;
unsigned long key2 = 0x006E66A4;


const unsigned char not_nhac_table[] = {29, 26, 23, 22, 19, 17, 15, 94, 91, 88, 86, 83, 81, 79};
const unsigned char happy_birthday[] = {
    S14, S14, L12, S12, D22, X11, S14, S14, L12, S12, R22,
    D21, S14, S14, S22, M22, D22, X12, L12, F24, F24, M22, D22,
    R22, D21, NOP, NOP, NOP,
    0
};
unsigned char SF = 0;
unsigned char AF = 0;
unsigned char sound_en;
unsigned char *not_nhac;
const unsigned char mute[] = {NOP, 0};
unsigned char sound_en_eeprom @ 0x4001;
unsigned char timer2_count = 0;

void play_sound(unsigned char *sound){
    BEEP_CSR_BEEPEN = 0;
    if(sound_en){
        not_nhac = sound;
        BEEP_CSR = not_nhac_table[(*not_nhac - 1) % 14];
        BEEP_CSR_BEEPEN = 1;
    }
    else{
        not_nhac = (unsigned char *)mute;
        BEEP_CSR = not_nhac_table[(*not_nhac - 1) % 14];
    }
    SF = 1;
}

void init_timer(){
    TIM1_ARRH = 0x00;
    TIM1_ARRL = 0x64;
    TIM1_PSCRH = 0x3E;
    TIM1_PSCRL = 0x7F;
    TIM1_IER = 0x01;
    TIM1_CR1 = 0x81;
    
  TIM2_ARRH = 0x07;
  TIM2_ARRL = 0xD0;
  TIM2_PSCR = 0x04;
  TIM2_IER = 0x01;
  TIM2_CR1 = 0x81;    
}
void main() {
	
	Clock_Configure();
	GPIO_Configure();
	RF433_Configure();
	
	LED7Seg_Configure();
	LED7Seg_Display(0);
	eeprom_unlock();
    init_timer();
    //asm("RIM");
    __enable_interrupt();
    static unsigned char key = 0;
    static unsigned long rd_data_old = 0, rf_count = 0;
    static unsigned long list[6] = {0,0,0,0,0,0};

//   M:\STM8\Firmwares- Tung (Clock) (1)
    static unsigned char countDemo = 0;
	while(1){
		if(RF.Data)
		{
			if(RF.Data == key1){				
                PA_ODR_ODR1 = !PA_ODR_ODR1;     //GATE1_PIN = !GATE1_PIN;
                countDemo++;
                countDemo %= 10;
                LED7Seg_Display(countDemo);
                
                AF = 1;
                
				while(RF.Hold);
			}
			
			if(RF.Data == key2){
				PA_ODR_ODR2 = !PA_ODR_ODR2;     //GATE2_PIN = !GATE2_PIN;
                countDemo--;
                if(countDemo == 0xFF) countDemo = 9;
                LED7Seg_Display(countDemo);                
				while(RF.Hold);
			}			
			RF.Data = 0;
		}
	}
    while(1){
        PA_ODR_ODR1 = 1;
        PA_ODR_ODR2 = 0;
        PA_ODR_ODR3 = 0;
        delay_ms(1000);
        
        PA_ODR_ODR1 = 0;
        PA_ODR_ODR2 = 1;
        PA_ODR_ODR3 = 0;
        delay_ms(1000);
        
        PA_ODR_ODR1 = 0;
        PA_ODR_ODR2 = 0;
        PA_ODR_ODR3 = 1;
        delay_ms(1000);
        static unsigned char count = 0;
        count++;
        LED7Seg_Display(count % 10);
    }
    
    while (1) {
        loop: if (RF.Data) {
            if (rd_data_old == RF.Data) {
                if (++rf_count == 5) {
                    rf_count = 0;

                    for (unsigned char i = 0; i < 6; i++) {
                        if (RF.Data == list[i]) {
                            RF.Data = 0;
                            goto loop;
                        }
                    }
                    list[key] = RF.Data;
                    if (++key >= 6) {
                        RF.Data = 0;
                        break;
                    }
                }
            } else
                rf_count = 1;
            rd_data_old = RF.Data;
            RF.Data = 0;
        }
    }
}


#pragma vector = TIM1_OVR_UIF_vector
__interrupt void TIM1_INTERRUPT() {
    static unsigned char * alarm_sound;
//    static unsigned char s = 0, m = 0;
    static unsigned char count = 0;
//    unsigned char temps;
//    IWDG_KR = 0xAA;

    if (AF) {
        if (count < 50) {
            if (!SF) {
                sound_en = 1;
                play_sound(alarm_sound);
                sound_en = ~sound_en_eeprom;
            }
        } else
            AF = 0;
    }
    TIM1_SR1 = 0x00;
}


#pragma vector = TIM2_OVR_UIF_vector
__interrupt void TIM2_INTERRUPT() {
//    static unsigned char scan = 0;
    static unsigned int count = 0;
//    static unsigned char button_count[3] = {
//        0,
//        0,
//        0
//    };
//    static unsigned char button_sample[3] = {
//        1,
//        1,
//        1
//    };
//    unsigned char temp;

    if (BEEP_CSR_BEEPEN || * not_nhac == NOP) {
        if (++count >= (( * not_nhac == NOP) ? 20 : (20 * (0x01 << (( * not_nhac - 1) / 14))))) {
            BEEP_CSR_BEEPEN = 0;
            not_nhac++;
            if ( * not_nhac) {
                if ( * not_nhac != NOP) {
                    BEEP_CSR = not_nhac_table[( * not_nhac - 1) % 14];
                    BEEP_CSR_BEEPEN = 1;
                }
            } else
                SF = 0;
            count = 0;
        }
    }
    if (timer2_count < 255)
        timer2_count++;
    TIM2_SR1 = 0x00;
}

/*------------------------------------------------------------------------
//------------------------------------------------------------------------*/
//#pragma vector = TIM2_OVR_UIF_vector
//__interrupt void Timer2_Interrupt(void)            
//{	
//	TIM2_SR1_UIF = 0; 		
////	PD_ODR_ODR6 = !PD_ODR_ODR6;
////	PD_ODR_ODR5 = !PD_ODR_ODR5;
//}

/*
                                      N76E003AT20/MS51FB9AE 
                                        ____________
              PWM2/IC6/T0/AIN4/P0.5|(01)			(20)|P0.4/AIN5/STADC/PWM3/IC3
                      TXD/AIN3/P0.6|(02)			(19)|P0.3/PWM5/IC5/AIN6
                      RXD/AIN2/P0.7|(03)			(18)|P0.2/ICPCK/OCDCK/RXD_1/[SCL]
                           RST/P2.0|(04)			(17)|P0.1/PWM4/IC4/MISO
               INT0/OSCIN/AIN1/P3.0|(05)			(16)|P0.0/PWM3/IC3/MOSI/T1
                     INT1/AIN0/P1.7|(06)			(15)|P1.0/PWM2/IC2/SPCLK
                                GND|(07)			(14)|P1.1/PWM1/IC1/AIN7/CLO
       [SDA]/TXD_1/ICPDA/OCDDA/P1.6|(08)			(13)|P1.2/PWM0/IC0
                                VDD|(09)			(12)|P1.3/SCL/[STADC]
                   PWM5/IC7/SS/P1.5|(10)___________ (11)|P1.4/SDA/FB/PWM1
       
*/

/*
KIT NUVOTON 1K                  
                                N76E003AT20/MS51FB9AE 
                                       ____________
                                3V3|(01)
                                3V3|(02)
                                5V0|(03)
                                5V0|(04)
                                GND|(05)            (24)|P0.4/AIN5/STADC/PWM3/IC3
                                GND|(06)            (23)|P0.3/PWM5/IC5/AIN6
              PWM2/IC6/T0/AIN4/P0.5|(07)			(22)|P0.2/ICPCK/OCDCK/RXD_1/[SCL]
                      TXD/AIN3/P0.6|(08)			(21)|P0.1/PWM4/IC4/MISO
                      RXD/AIN2/P0.7|(09)			(20)|P0.0/PWM3/IC3/MOSI/T1
               INT0/OSCIN/AIN1/P3.0|(10)			(19)|P1.0/PWM2/IC2/SPCLK
                     INT1/AIN0/P1.7|(11)			(18)|P1.1/PWM1/IC1/AIN7/CLO
       [SDA]/TXD_1/ICPDA/OCDDA/P1.6|(12)			(17)|P1.2/PWM0/IC0
                   PWM5/IC7/SS/P1.5|(13)            (16)|P1.3/SCL/[STADC]
                           RST/P2.0|(14)___________ (15)|P1.4/SDA/FB/PWM1
*/

/*
                                      STM8S003F3P5
                                      ____________
     UART1_CK/TIM2_CH1/BEEP/(HS)PD4|(01)			(20)|PD3(HS)/AIN4/TIM2_CH2/ADC_ETR
              UART1_TX/AIN5/(HS)PD5|(02)			(19)|PD2(HS)/AIN3/[TIM2_CH3]
               UART1_RX/AIN6(HS)PD6|(03)			(18)|PD1(HS)/SWIM
                               NRST|(04)			(17)|PC7/(HS)/SPI_MISO[TIM1_CH2]
                          OSCIN/PA1|(05)			(16)|PC6(HS)/SPI_MOSI[TIM1_CH1]
                         OSCOUT/PA2|(06)			(15)|PC5(HS)/SPI_SCK[TIM2_CH1]
                                GND|(07)			(14)|PC4(HS)/TIM1_CH4/CLK_CCO/AIN2/[TIM1_CH2N]
                               VCAP|(08)			(13)|PC3(HS)/TIM1_CH3[TLI][TIM1_CH1N]
                                VDD|(09)			(12)|(Open Drain)PB4(T)/I2C_SCL[   ADC_ETR ]
          [SPI_NSS]TIM2_CH3/(HS)PA3|(10)__________  (11)|(Open Drain)PB5(T)/I2C_SDA[TIM1_BKIN]
*/

/*
        KIT NUVOTON 1K                  STM8S003F3
                                        ____________
                                3V3|(01)
                                3V3|(02)
                                5V0|(03)
                                5V0|(04)
                                GND|(05)           (24)|PD3(HS)/AIN4/TIM2_CH2/ADC_ETR
                                GND|(06)           (23)|PD2(HS)/AIN3/[TIM2_CH3]
     UART1_CK/TIM2_CH1/BEEP/(HS)PD4|(07)		   (22)|PD1(HS)/SWIM
              UART1_TX/AIN5/(HS)PD5|(08)		   (21)|PC7/(HS)/SPI_MISO[TIM1_CH2]
               UART1_RX/AIN6(HS)PD6|(09)		   (20)|PC6(HS)/SPI_MOSI[TIM1_CH1]
                          OSCIN/PA1|(10)		   (19)|PC5(HS)/SPI_SCK[TIM2_CH1]
                         OSCOUT/PA2|(11)		   (18)|PC4(HS)/TIM1_CH4/CLK_CCO/AIN2/[TIM1_CH2N]
                               VCAP|(12)		   (17)|PC3(HS)/TIM1_CH3[TLI][TIM1_CH1N]
          [SPI_NSS]TIM2_CH3/(HS)PA3|(13)           (16)|(Open Drain)PB4(T)/I2C_SCL[   ADC_ETR ]
                               NRST|(14)___________(15)|(Open Drain)PB5(T)/I2C_SDA[TIM1_BKIN]

*/


/*------------------------------------------------------------------------
----------------------------------------------------------------------*/
