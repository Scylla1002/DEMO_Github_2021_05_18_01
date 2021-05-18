#include "BEE_STM8S_HW_Configure.h"
#include  <iostm8s003f3.h>


void eeprom_unlock(){
    FLASH_DUKR = 0xAE;
    FLASH_DUKR = 0x56;
}
/*-----------------------------------------------------------------------------
ODR;DDR;IDR;CR1;CR2
DDR |   CR1 |   CR2 |
-----------------------------
0   |   0   |   0   |Input_Float_NoInterrupt
0   |   0   |   1   |Input_Float_Interrupt
0   |   1   |   0   |Input_Pullup_NoInterrupt
0   |   1   |   1   |Input_Pullup_Interrupt
--------------------
1   |   0   |   0   |Output_OpenDrain_2MHz
1   |   0   |   1   |Output_OpenDrain_10MHz
1   |   1   |   0   |Output_Pushpull_2MHz
1   |   1   |   1   |Output_Pushpull_10MHz
-----------------------------------------------------------------------------*/

void GPIO_Configure()
{
	PA_ODR = 0x00;
	PA_DDR = 0x00;
	PA_CR1 = 0x00;
	PA_CR2 = 0x00;

	PB_ODR = 0xFF;
	PB_DDR = 0xFF;
	PB_CR1 = 0xFF;
	PB_CR2 = 0xFF;
	
	PC_ODR = 0xFF;
	PC_DDR = 0xFF;
	PC_CR1 = 0xFF;
	PC_CR2 = 0xFF;	
	
	PD_ODR = 0x01;
	PD_DDR = 0x01;
	PD_CR1 = 0x01;
	PD_CR2 = 0x01;
	
	PE_ODR = 0xFF;
	PE_DDR = 0xFF;
	PE_CR1 = 0xFF;
	PE_CR2 = 0xFF;

	PF_ODR = 0x00;
	PF_DDR = 0x00;
	PF_CR1 = 0x00;
	PF_CR2 = 0x00;		
    
//	PG_ODR = 0xFF;
//	PG_DDR = 0xFF;
//	PG_CR1 = 0xFF;
//	PG_CR2 = 0xFF;	
    
    PD_ODR |= (1 << 3);
    PD_DDR |= (1 << 3);
    PD_CR1 |= (1 << 3);
    PD_CR2 |= (1 << 3);
	
	
	GATE1_DDR = 1;
	GATE1_CR1 = 1;
	GATE1_CR2 = 1;
	
	GATE2_DDR = 1;
	GATE2_CR1 = 1;
	GATE2_CR2 = 1;	
    
    //LED_Output
    PA_ODR &= ~0x0E;
    PA_DDR |= 0x0E;
    PA_CR1 |= 0x0E;
    PA_CR2 |= 0x0E;
}

void Clock_Configure()
{
	CLK_ICKR_HSIEN 	     = 1;
	CLK_ECKR_HSEEN 	     = 0;
	CLK_CKDIVR_HSIDIV    = 0x00;   
	CLK_SWCR_SWEN 	     = 1;
	CLK_SWR              = 0xE1; 	
}


/*------------------------------------------------------------------------
------------------------------------------------------------------------*/
unsigned int		Timer2_AutoReload_Value = 210;

void TIM2_Configure()
{
	TIM2_PSCR                   = 0;
	TIM2_ARRH                   = (Timer2_AutoReload_Value >> 8);
	TIM2_ARRL                   = (Timer2_AutoReload_Value & 0xFF);
	TIM2_CR1_ARPE               = 1;
	TIM2_IER_UIE                = 1;
	TIM2_CR1_CEN                = 1;   	
}

#define		Fosc							16000000
#define     F_PWM_TIM2                   	2000            		//Tan so PWM (Thay doi duoc tu 100 Hz den 8 KHz)
#define     TIM2_Prescaler_Value        	(0)                     //He so chia [0..15]
#define     TIM2_AutoReload_Value       	TIM2_Reload_Value       
#define     TIM2_CCR_Value              	TIM2_Reload_Value/20 
#define     TIM2_Reload_Value           	(unsigned long)(Fosc/F_PWM_TIM2)


void TIM2_PWM_Configure()
{
    TIM2_PSCR = TIM2_Prescaler_Value;
    
    TIM2_ARRH = ((unsigned int)TIM2_AutoReload_Value >> 8);
    TIM2_ARRL = (TIM2_AutoReload_Value & 0xFF); 

    /*Thanh ghi dem ???*/
    TIM2_CNTRH = 0;
    TIM2_CNTRL = 0;    
    
    TIM2_CR1_ARPE = 1;  //Preload enable
    //TIM3_IER_UIE = 1;   //Cho phep ngat update	
	
    TIM2_CCER1_CC1E = 1;
    TIM2_CCER1_CC1P = 0;
    TIM2_CCMR1_OC1M = 5;     //PWM1 mode
//    TIM2_CCMR1_OC1M = 4;     //Force inactive level
    
    TIM2_CCR1H = (TIM2_CCR_Value >> 8);
    TIM2_CCR1L = (TIM2_CCR_Value & 0xFF);	
	
    TIM2_CR1_CEN = 1;   //Timer2 Enable	
}

/*-----------------------------------------------------------------------------
Chanel 1 (TIM1-CH1)
-----------------------------------------------------------------------------*/
#define     TIM1_CCR_Value      		500   					//Duty Cycle
#define     TIM1_Prescaler_Value        (16-1)                  //(16000 - 1)      //F-Timer = 1KHz
#define     TIM1_AutoReload_Value       1000                    //Gia tri nap lai

void TIM1_PWM1_Configure()
{
    /* Set the Autoreload value */
    TIM1_ARRH = ((unsigned int)TIM1_AutoReload_Value >> 8);
    TIM1_ARRL = (TIM1_AutoReload_Value & 0xFF);  
    
    /*Thanh ghi dem ???*/
    //TIM1_CNTRH = 0xFF;
    //TIM1_CNTRL = 0xFF;
    
    /* Set the Prescaler value*/
    TIM1_PSCRH = (TIM1_Prescaler_Value >> 8);
    TIM1_PSCRL = (TIM1_Prescaler_Value & 0xFF);    
    
    /* Select the Counter Mode 
    0:Edge-aligned mode.
    1:Center-aligned mode 1
    2:Center-aligned mode 2
    3:Center-aligned mode 3
    */
    TIM1_CR1_CMS = 0x00;
    
    /* 0: Count-Up; 1: Count-Down */
    TIM1_CR1_DIR = 0;//Up
    
    /* Set the Repetition Counter value */
    TIM1_RCR = 0;   //Sau bao nhiêu lần tràn trước khi set cờ ngắt ???
    //-----------------------
 
/* Disable the Channel 1: Reset the CCE Bit, Set the Output State , 
        the Output N State, the Output Polarity & the Output N Polarity*/    
    //TIM1_CCER1_CC1E = 0;
    //TIM1_CCER1_CC1NE = 0;
    //TIM1_CCER1_CC1P = 0;
    //TIM1_CCER1_CC1NP = 0;
    
/* Set the Output State & Set the Output N State & Set the Output Polarity &
  Set the Output N Polarity */
        
    TIM1_CCER1_CC1E = 1;
    //TIM1_CCER1_CC1NE = 0;
    TIM1_CCER1_CC1P = 0;
    //TIM1_CCER1_CC1NP = 0;    

/* Reset the Output Idle state & the Output N Idle state bits */
    TIM1_CCMR1_OC1M = 0x06;     //PWM1 mode
    //TIM1_CCMR1_OC1M = 0x04;     //PWM1 force inactive

/* Set the Output Idle state & the Output N Idle state configuration */    
    //TIM1_OISR_OIS1 = 0;
    //TIM1_OISR_OIS1N = 0;
      
 
 /* Set the Pulse value */
    TIM1_CCR1H = (TIM1_CCR_Value >> 8);
    TIM1_CCR1L = (TIM1_CCR_Value & 0xFF);
    
    /*Main output enable*/
    TIM1_BKR_MOE = 1;         //Cho phep PWM chay
    //TIM1_BKR_MOE = 0;           //Chua cho phep PWM chay
    //-----------------------
    TIM1_CR1_ARPE = 1;          //Enable Preload
    
    //TIM1_IER_UIE = 1;         //Cho phép ngắt Update    
    TIM1_CR1_CEN = 1;           //TIM1 enable
}

void TIM1_PWM2_Configure()
{
    /* Set the Autoreload value */
    TIM1_ARRH = ((unsigned int)TIM1_AutoReload_Value >> 8);
    TIM1_ARRL = (TIM1_AutoReload_Value & 0xFF);  
        
    /* Set the Prescaler value*/
    TIM1_PSCRH = (TIM1_Prescaler_Value >> 8);
    TIM1_PSCRL = (TIM1_Prescaler_Value & 0xFF);    
    
    /* Select the Counter Mode 
    0:Edge-aligned mode.
    1:Center-aligned mode 1
    2:Center-aligned mode 2
    3:Center-aligned mode 3
    */
    TIM1_CR1_CMS = 0x00;
    
    /* 0: Count-Up; 1: Count-Down */
    TIM1_CR1_DIR = 0;//Up
    
    /* Set the Repetition Counter value */
    TIM1_RCR = 0;
    //-----------------------
 
/* Disable the Channel 1: Reset the CCE Bit, Set the Output State , 
        the Output N State, the Output Polarity & the Output N Polarity*/    
    TIM1_CCER1_CC2E = 0;
    TIM1_CCER1_CC2NE = 0;
    TIM1_CCER1_CC2P = 0;
    TIM1_CCER1_CC2NP = 0;
    
/* Set the Output State & Set the Output N State & Set the Output Polarity &
  Set the Output N Polarity */
        
    TIM1_CCER1_CC2E = 1;
    //TIM1_CCER1_CC2NE = 0;
    TIM1_CCER1_CC2P = 0;
    //TIM1_CCER1_CC2NP = 0;    
    

/* Reset the Output Idle state & the Output N Idle state bits */
    TIM1_CCMR2_OC2M = 0x06;     //PWM1 mode
    //TIM1_CCMR1_OC2M = 0x04;     //PWM1 force inactive

/* Set the Output Idle state & the Output N Idle state configuration */    
    //TIM1_OISR_OIS2 = 0;
    //TIM1_OISR_OIS2N = 0;
      
 
 /* Set the Pulse value */
    TIM1_CCR2H = (TIM1_CCR_Value >> 8);
    TIM1_CCR2L = (TIM1_CCR_Value & 0xFF);
    
    /*Main output enable*/
    TIM1_BKR_MOE = 1;         //Cho phep PWM chay
    //TIM1_BKR_MOE = 0;           //Chua cho phep PWM chay
    //-----------------------
    TIM1_CR1_ARPE = 1;          //Enable Preload
    
    //TIM1_IER_UIE = 1;         //Cho phép ngắt Update    
    TIM1_CR1_CEN = 1;           //TIM1 enable
}

void TIM1_PWM3_Configure()
{
    /* Set the Autoreload value */
    TIM1_ARRH = ((unsigned int)TIM1_AutoReload_Value >> 8);
    TIM1_ARRL = (TIM1_AutoReload_Value & 0xFF);  
    
    /* Set the Prescaler value*/
    TIM1_PSCRH = (TIM1_Prescaler_Value >> 8);
    TIM1_PSCRL = (TIM1_Prescaler_Value & 0xFF);    
    
    /* Select the Counter Mode 
    0:Edge-aligned mode.
    1:Center-aligned mode 1
    2:Center-aligned mode 2
    3:Center-aligned mode 3
    */
    TIM1_CR1_CMS = 0x00;
    
    /* 0: Count-Up; 1: Count-Down */
    TIM1_CR1_DIR = 0;//Up
    
    /* Set the Repetition Counter value */
    TIM1_RCR = 0;
    //-----------------------
 
/* Disable the Channel 1: Reset the CCE Bit, Set the Output State , 
        the Output N State, the Output Polarity & the Output N Polarity*/    
    TIM1_CCER2_CC3E = 0;
    TIM1_CCER2_CC3NE = 0;
    TIM1_CCER2_CC3P = 0;
    TIM1_CCER2_CC3NP = 0;
    
/* Set the Output State & Set the Output N State & Set the Output Polarity &
  Set the Output N Polarity */
        
    TIM1_CCER2_CC3E = 1;
//    TIM1_CCER2_CC3NE = 0;
    TIM1_CCER2_CC3P = 0;
//    TIM1_CCER2_CC3NP = 0;    
    
/* Reset the Output Idle state & the Output N Idle state bits */
    TIM1_CCMR3_OC3M = 0x06;     //PWM1 mode

/* Set the Output Idle state & the Output N Idle state configuration */    
    //TIM1_OISR_OIS3 = 0;
    //TIM1_OISR_OIS3N = 0;
      
 
 /* Set the Pulse value */
    TIM1_CCR3H = (TIM1_CCR_Value >> 8);
    TIM1_CCR3L = (TIM1_CCR_Value & 0xFF);
    
    /*Main output enable*/
    TIM1_BKR_MOE = 1;         //Cho phep PWM chay
    //TIM1_BKR_MOE = 0;           //Chua cho phep PWM chay
    //-----------------------
    TIM1_CR1_ARPE = 1;          //Enable Preload
    
    //TIM1_IER_UIE = 1;         //Cho phép ngắt Update    
    TIM1_CR1_CEN = 1;           //TIM1 enable
}

void TIM1_PWM4_Configure()
{
    /* Set the Autoreload value */
    TIM1_ARRH = ((unsigned int)TIM1_AutoReload_Value >> 8);
    TIM1_ARRL = (TIM1_AutoReload_Value & 0xFF);  
    
    /* Set the Prescaler value*/
    TIM1_PSCRH = (TIM1_Prescaler_Value >> 8);
    TIM1_PSCRL = (TIM1_Prescaler_Value & 0xFF);    
    
    /* Select the Counter Mode 
    0:Edge-aligned mode.
    1:Center-aligned mode 1
    2:Center-aligned mode 2
    3:Center-aligned mode 3
    */
    TIM1_CR1_CMS = 0x00;
    
    /* 0: Count-Up; 1: Count-Down */
    TIM1_CR1_DIR = 0;//Up
    
    /* Set the Repetition Counter value */
    TIM1_RCR = 0;
    //-----------------------
 
/* Disable the Channel 1: Reset the CCE Bit, Set the Output State , 
        the Output N State, the Output Polarity & the Output N Polarity*/    
    TIM1_CCER2_CC4E = 0;
    TIM1_CCER2_CC4P = 0;
    
/* Set the Output State & Set the Output N State & Set the Output Polarity &
  Set the Output N Polarity */
        
    TIM1_CCER2_CC4E = 1;
    TIM1_CCER2_CC4P = 0;
    
/* Reset the Output Idle state & the Output N Idle state bits */
    TIM1_CCMR3_OC3M = 0x06;     //PWM1 mode

/* Set the Output Idle state & the Output N Idle state configuration */    
    TIM1_OISR_OIS4 = 0;
    //TIM1_OISR_OIS3N = 0;
      
 
 /* Set the Pulse value */
    TIM1_CCR3H = (TIM1_CCR_Value >> 8);
    TIM1_CCR3L = (TIM1_CCR_Value & 0xFF);
    
    /*Main output enable*/
    TIM1_BKR_MOE = 1;         		//Cho phep PWM chay
    //TIM1_BKR_MOE = 0;           	//Chua cho phep PWM chay
    //-----------------------
    TIM1_CR1_ARPE = 1;          	//Enable Preload
    
    //TIM1_IER_UIE = 1;         	//Cho phép ngắt Update    
    TIM1_CR1_CEN = 1;           	//TIM1 enable
}

/*-----------------------------------------------------------------------------
PC1 (PIN18) TIM1_CH1
PB0 (PIN16) [TIM1_CH1N]->Option Byte
-----------------------------------------------------------------------------*/
unsigned int PWM_Duty = 10;
unsigned int PWM_Duty1 = 10;
unsigned int PWM_Duty2 = 15;
unsigned int PWM_Duty3 = 20;

float DeadTime_Factor = 0.5;
#define     TIM1_CH1_DeadTime                       10

void TIM1_PWM_Complementary_Configure()
{
    PWM_Duty = (unsigned int)((TIM1_AutoReload_Value / 2) * DeadTime_Factor);

    TIM1_ARRH = ((unsigned int)TIM1_AutoReload_Value >> 8);
    TIM1_ARRL = (TIM1_AutoReload_Value & 0xFF);  
    
    
    /*Thanh ghi dem ???*/
    //TIM1_CNTRH = 0xFF;
    //TIM1_CNTRL = 0xFF;
    
    /* Set the Prescaler value*/
    TIM1_PSCRH = (TIM1_Prescaler_Value >> 8);
    TIM1_PSCRL = (TIM1_Prescaler_Value & 0xFF);    
    
    /* Select the Counter Mode 
    0:Edge-aligned mode.
    1:Center-aligned mode 1
    2:Center-aligned mode 2
    3:Center-aligned mode 3
    */
    TIM1_CR1_CMS = 0x01;    //PWM center-aligned mode
    
    /* 0: Count-Up; 1: Count-Down */
    TIM1_CR1_DIR = 1;//Up
    
    /* Set the Repetition Counter value */
    TIM1_RCR = 0;
    //-----------------------

    
    /* Set the Output State & Set the Output N State & Set the Output Polarity &
    Set the Output N Polarity */
        
    TIM1_CCER1_CC1E     = 1;        //Cho phep dau ra TIM1_CH1
    TIM1_CCER1_CC1NE    = 1;        //Cho phep dau ra TIM1_CH1N
    //TIM1_CCER1_CC1P     = 1;        //Dau ra TIM1_CH1 Active-High
    //TIM1_CCER1_CC1NP    = 1;        //Dau ra TIM1_CH1N  Active-Low    

    TIM1_CCER1_CC2E     = 1;        //Cho phep dau ra TIM1_CH2
    TIM1_CCER1_CC2NE    = 1;        //Cho phep dau ra TIM1_CH2N
    //TIM1_CCER1_CC1P     = 1;        //Dau ra TIM1_CH1 Active-High
    //TIM1_CCER1_CC1NP    = 1;        //Dau ra TIM1_CH1N  Active-Low  

    TIM1_CCER2_CC3E     = 1;        //Cho phep dau ra TIM1_CH1
    TIM1_CCER2_CC3NE    = 1;        //Cho phep dau ra TIM1_CH1N
    //TIM1_CCER2_CC3P     = 1;        //Dau ra TIM1_CH1 Active-High
    //TIM1_CCER2_CC3NP    = 1;        //Dau ra TIM1_CH1N  Active-Low  
	
    /* Set the Output State & Set the Output N State & Set the Output Polarity &
    Set the Output N Polarity */
        
    TIM1_CCER2_CC4E     = 1;        //Cho phep dau ra TIM1_CH4
    //TIM1_CCER2_CC4P     = 1;        //Dau ra TIM1_CH4 Active-High
    
    
    /* Reset the Output Idle state & the Output N Idle state bits */
    TIM1_CCMR1_OC1M = 0x07;     //PWM1 mode
    TIM1_CCMR4_OC4M = 0x06;

    /* Set the Output Idle state & the Output N Idle state configuration */    
    //TIM1_OISR_OIS1 = 0;
    //TIM1_OISR_OIS1N = 0;
      
 
    /*Pulse Width*/
    TIM1_CCR1H = (PWM_Duty1 >> 8);
    TIM1_CCR1L = (PWM_Duty1 & 0xFF);

	TIM1_CCR2H = (PWM_Duty2 >> 8);
    TIM1_CCR2L = (PWM_Duty2 & 0xFF);

    TIM1_CCR3H = (PWM_Duty3 >> 8);
    TIM1_CCR3L = (PWM_Duty3 & 0xFF);
	
    TIM1_CCR4H = ((TIM1_AutoReload_Value-PWM_Duty) >> 8);
    TIM1_CCR4L = ((TIM1_AutoReload_Value-PWM_Duty) & 0xFF);
    
    TIM1_BKR_MOE = 1;       //Cho phep PWM chay
    //TIM1_BKR_MOE = 0;       //Chua cho phep PWM chay
    //-----------------------
    TIM1_CR1_ARPE = 1;  //Enable Preload
    
    //TIM1_IER_UIE = 1;   //Cho phép ngắt Update
    
    TIM1_DTR = TIM1_CH1_DeadTime;
    TIM1_CR1_CEN = 1;   //TIM1 enable
}

#define		TIM4_AutoReload_Value		200

void TIM4_Configure()
{
    TIM4_PSCR = TIM2_Prescaler_Value;
    
    TIM4_ARR = TIM4_AutoReload_Value;
 
    /*Thanh ghi dem ???*/
    TIM4_CNTR = 0;  
    
    TIM4_CR1_ARPE = 1;  //Preload enable
    //TIM3_IER_UIE = 1;   //Cho phep ngat update	
	
    TIM4_CR1_CEN = 1;   //Timer2 Enable	
}

/*------------------------------------------------------------------------
------------------------------------------------------------------------*/