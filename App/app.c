// 
// Create task and start OS 
//
//
 
#include <includes.h>

INT16U led_stat=0;
INT16U SW1_logic=1;
    
#define TASK_1_PRIO       1
#define TASK_2_PRIO       4
#define TASK_3_PRIO       2
#define TASK_4_PRIO       3

#define TASK_1_STK_SIZE   256
OS_STK Task_1_Stk[TASK_1_STK_SIZE];   /* Task stack */

#define TASK_2_STK_SIZE   256
OS_STK Task_2_Stk[TASK_2_STK_SIZE];   /* Task stack */

#define TASK_1_STK_SIZE   256
OS_STK Task_3_Stk[TASK_3_STK_SIZE];   /* Task stack */

#define TASK_2_STK_SIZE   256
OS_STK Task_4_Stk[TASK_4_STK_SIZE];   /* Task stack */

static void Task_1(void *p_arg);      /* Task function */
static void Task_2(void *p_arg);      /* Task function */
static void Task_3(void *p_arg);      /* Task function */
static void Task_4(void *p_arg);      /* Task function */


CPU_INT16S  main (void)
{  
  CPU_INT08U err;
  
  OSInit();
  BSP_Init();  
  
  OSTaskCreate(Task_1, (void *)0, (OS_STK *)&Task_1_Stk[0], TASK_1_PRIO);
  OSTaskCreate(Task_2, (void *)0, (OS_STK *)&Task_2_Stk[0], TASK_2_PRIO);
  
  OSTaskNameSet(TASK_1_PRIO,  (CPU_INT08U *)"Task 1", &err);
  OSTaskNameSet(TASK_2_PRIO,  (CPU_INT08U *)"Task 2", &err);
    
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
		OSTimeDlyHMSM(0,0,0,20);
	}

	if(SW2_d!=SW2)
	{
		if(SW2_d==0)
			SW2=0;
		if(SW2_d==1)
			SW2=1;
		OSTimeDlyHMSM(0,0,0,20);
	}
	OSTimeDlyHMSM(0,0,0,5);
  }
}

static void Task_2(void *p_arg)	//led 3 blinking at 1 hz
{
  (void)p_arg;  /* avoid compiler warning */
  
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
		if(SW1==0)
		{
			if(SW1_logic==1)
			{
				if(led_stat<3)
				{
					led_stat=led_stat+1;
				}
				if(led_stat==3)
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
				OSTimeDlyHMSM(0,0,5,0);
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