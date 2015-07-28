// 
// Create task and start OS 
//
//
 
#include <includes.h>

INT32U i = 0;
INT32U j = 0;
    
#define TASK_1_PRIO       8
#define TASK_2_PRIO       5

#define TASK_1_STK_SIZE   256
OS_STK Task_1_Stk[TASK_1_STK_SIZE];   /* Task stack */

#define TASK_2_STK_SIZE   256
OS_STK Task_2_Stk[TASK_2_STK_SIZE];   /* Task stack */

static void Task_1(void *p_arg);      /* Task function */
static void Task_2(void *p_arg);      /* Task function */


CPU_INT16S  main (void)
{  
  CPU_INT08U err;
  
  OSInit();  
  
  OSTaskCreate(Task_1, (void *)0, (OS_STK *)&Task_1_Stk[0], TASK_1_PRIO);
  OSTaskCreate(Task_2, (void *)0, (OS_STK *)&Task_2_Stk[0], TASK_2_PRIO);
  
  OSTaskNameSet(TASK_1_PRIO,  (CPU_INT08U *)"Task 1", &err);
  OSTaskNameSet(TASK_2_PRIO,  (CPU_INT08U *)"Task 2", &err);
    
  OSStart();                     

	return (-1);             
}

static void Task_1(void *p_arg)
{
  (void)p_arg;  /* avoid compiler warning */
  
  while (1) {
    i++;
    i++;
  }
}

static void Task_2(void *p_arg)
{
  (void)p_arg;  /* avoid compiler warning */
  
  while (1) {
    j++;
    j++;  
  }
}
