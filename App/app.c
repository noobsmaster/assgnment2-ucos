// 
// Create task and start OS 
//
//
 
#include <includes.h>
#include "Param.h"
#include "IOCon.h"
#include "ADC.h"

INT16U led_stat=0;
INT16U SW1_logic=1;
INT8U avgADCValue = 0;
unsigned int ADCValue, count;
volatile unsigned int *ADCPtr;
    
#define TASK_1_PRIO       1
#define TASK_2_PRIO       4
#define TASK_3_PRIO       2
#define TASK_4_PRIO       3
#define TASK_5_PRIO       5
#define TASK_6_PRIO       6


#define TASK_1_STK_SIZE   256
OS_STK Task_1_Stk[TASK_1_STK_SIZE];   /* Task stack */

#define TASK_2_STK_SIZE   256
OS_STK Task_2_Stk[TASK_2_STK_SIZE];   /* Task stack */

#define TASK_3_STK_SIZE   256
OS_STK Task_3_Stk[TASK_3_STK_SIZE];   /* Task stack */

#define TASK_4_STK_SIZE   256
OS_STK Task_4_Stk[TASK_4_STK_SIZE];   /* Task stack */

#define TASK_5_STK_SIZE   256
OS_STK Task_5_Stk[TASK_5_STK_SIZE];   /* Task stack */

#define TASK_6_STK_SIZE   256
OS_STK Task_6_Stk[TASK_6_STK_SIZE];   /* Task stack */

static void Task_1(void *p_arg);      /* Task function */
static void Task_2(void *p_arg);      /* Task function */
static void Task_3(void *p_arg);      /* Task function */
static void Task_4(void *p_arg);      /* Task function */
static void Task_5(void *p_arg);      /* Task function */
static void Task_6(void *p_arg);      /* Task function */


CPU_INT16S  main (void)
{  
  CPU_INT08U err;
  
  OSInit();
  BSP_Init();
  IO_Init();
  ADC_Init();  
  
  OSTaskCreate(Task_1, (void *)0, (OS_STK *)&Task_1_Stk[0], TASK_1_PRIO);
  OSTaskCreate(Task_2, (void *)0, (OS_STK *)&Task_2_Stk[0], TASK_2_PRIO);
  OSTaskCreate(Task_3, (void *)0, (OS_STK *)&Task_3_Stk[0], TASK_3_PRIO);
  OSTaskCreate(Task_4, (void *)0, (OS_STK *)&Task_4_Stk[0], TASK_4_PRIO);
  OSTaskCreate(Task_5, (void *)0, (OS_STK *)&Task_5_Stk[0], TASK_5_PRIO);
  OSTaskCreate(Task_6, (void *)0, (OS_STK *)&Task_6_Stk[0], TASK_6_PRIO);
  
  OSTaskNameSet(TASK_1_PRIO,  (CPU_INT08U *)"Task 1", &err);
  OSTaskNameSet(TASK_2_PRIO,  (CPU_INT08U *)"Task 2", &err);
  OSTaskNameSet(TASK_3_PRIO,  (CPU_INT08U *)"Task 3", &err);
  OSTaskNameSet(TASK_4_PRIO,  (CPU_INT08U *)"Task 4", &err);
  OSTaskNameSet(TASK_5_PRIO,  (CPU_INT08U *)"Task 5", &err);
  OSTaskNameSet(TASK_6_PRIO,  (CPU_INT08U *)"Task 6", &err);
    
  OSStart();                     

	return (-1);             
}

static void Task_1(void *p_arg) //task 1 = de-bouncing
{
  (void)p_arg;  /* avoid compiler warning */
  
  while (1) 
  {
  	if(SW1_d!=SW1)
	{
		if(SW1_d==0)
			SW1=0;
		if(SW1_d==1)
			SW1=1;
		OSTimeDlyHMSM(0,0,0,15);
	}

	if(SW2_d!=SW2)
	{
		if(SW2_d==0)
			SW2=0;
		if(SW2_d==1)
			SW2=1;
		OSTimeDlyHMSM(0,0,0,15);
	}
	OSTimeDlyHMSM(0,0,0,5);
  }
}

static void Task_2(void *p_arg)	//led 3 blinking at 1 hz
{
  (void)p_arg;  /* avoid compiler warning */
  LED3=0;
  while (1) 
  {
    LED3=~LED3;
	OSTimeDlyHMSM(0,0,0,500);
  }
}

static void Task_3(void *p_arg)	//led 1 blinking state control
{
	(void)p_arg;  /* avoid compiler warning */
	
	while (1)
	{
		if(SW1==1)
		{
			SW1_logic=1;
		}
		else if(SW1==0)
		{
			if(SW1_logic==1)
			{
				if(led_stat<=3)
				{
					led_stat=led_stat+1;
				}
				if(led_stat>3)
				{
					led_stat=0;
				}
				OSTimeDlyResume(TASK_4_PRIO);
			
			}
			SW1_logic=0;
		}
		OSTimeDlyHMSM(0,0,0,10);
	}
	
}

static void Task_4(void *p_arg)	//led 1 blinking timing
{
	(void)p_arg;  /* avoid compiler warning */
	
	while (1)
	{
		switch(led_stat)
		{
			case(0):
				LED1=0;
				OSTimeDlyHMSM(0,0,1,0);
				break;
			case(1):
				LED1=~LED1;
				OSTimeDlyHMSM(0,0,0,500);
				break;
			case(2):
				LED1=~LED1;
				OSTimeDlyHMSM(0,0,0,250);
				break;
			case(3):
				LED1=~LED1;
				OSTimeDlyHMSM(0,0,0,125);
				break;
				
		}
	}
}

static void Task_5(void *p_arg) //ADC
{
	while (1)
	{
		if(SW2==0)
		{
			ADCValue = 0;                         //reset ADC value
			ADCPtr = (unsigned int *)&ADC1BUF0;   // initialize ADC1BUF pointer
			IFS0bits.AD1IF = 0;                   // clear ADC interrupt flag
			AD1CON1bits.ASAM = 1;                 //run sampling indefinitely
			while (!IFS0bits.AD1IF){};            //ADC module only proceed when ADC interrupt is triggered
			AD1CON1bits.ASAM = 0;                 //stop sampling
			for(count = 0; count < 2; count++)		//averaging ADC sample
			{
				ADCValue = ADCValue + *ADCPtr++;
			}
			ADCValue = ADCValue >> 1;		//divide two
			avgADCValue = ADCValue;
			
			OSTimeDlyResume(TASK_6_PRIO);
			OSTimeDlyHMSM(0,0,0,200);

		}
		else if(SW2==1)
		{
			avgADCValue = 0;
		}
		OSTimeDlyHMSM(0,0,0,20);
	}
}

static void Task_6(void *p_arg)	//led 2 blinking timing
{
	LED2=0;
	while (1)
	{
		if(avgADCValue>10&&avgADCValue<=1023)
		{
			LED1=~LED1;
			OSTimeDlyHMSM(0,0,0,(avgADCValue*10));
		}
		else
		{
			LED2=0;
			OSTaskSuspend(OS_PRIO_SELF);
		}
	}
}