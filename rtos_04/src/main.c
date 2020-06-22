/* configUSE_IDLE_HOOK must be set to 1 in FreeRTOSConfig.h for the idle hook
function to get called */

#include "stdio.h"

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* LPCOpen includes*/
#include "board.h"

/* Demo includes. */
#include "supporting_functions.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT            ( 0xffffff )

/* Tasks priority */
#define PRIO_T1 2
#define PRIO_T2 1

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/* idle hook function*/
void vApplicationIdleHook( void );

/*-----------------------------------------------------------*/

int main( void )
{
   /* Create one of the two tasks. */
   xTaskCreate( vTask1, (const char *)"Task 1", configMINIMAL_STACK_SIZE, NULL, PRIO_T1, NULL );
   xTaskCreate( vTask2, (const char *)"Task 2", configMINIMAL_STACK_SIZE, NULL, PRIO_T2, NULL );
   /* Start the scheduler to start the tasks executing. */
   vTaskStartScheduler();

   /* The following line should never be reached because vTaskStartScheduler()
   will only return if there was not enough FreeRTOS heap memory available to
   create the Idle and (if configured) Timer tasks.  Heap management, and
   techniques for trapping heap exhaustion, are described in the book text. */
   for( ;; );
   return 0;
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
  const char *pcTaskName = "Task 2 is running. Priority \r\n";
  const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL ); //500 ms
  UBaseType_t uxPriority;
  char *pcTaskPriority;

  uxPriority = uxTaskPriorityGet( NULL );
  pcTaskPriority = (char *) uxPriority;

  /* As per most tasks, this task is implemented in an infinite loop. */
  for( ;; ) {
     /* Print out the name of this task. */
     vPrintString( pcTaskName );
     vPrintString( pcTaskPriority );
     vPrintString( "\r\n" );

     vTaskDelay( xDelay500ms );
  }
}
/*-----------------------------------------------------------*/


void vTask2( void *pvParameters )
{
   const char *pcTaskName = "Task 2 is running. Priority \r\n";
   const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL ); //500 ms
   UBaseType_t uxPriority;
   char *pcTaskPriority;

   uxPriority = uxTaskPriorityGet( NULL );
   pcTaskPriority = (char *) uxPriority;

   /* As per most tasks, this task is implemented in an infinite loop. */
   for( ;; ) {
      /* Print out the name of this task. */
      vPrintString( pcTaskName );
      vPrintString( pcTaskPriority );
      vPrintString( "\r\n" );

      vTaskDelay( xDelay500ms );
   }
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xDelay300ms = 30; //1 seg


  if (xTaskGetTickCount() - xLastWakeTime >= xDelay300ms){ //aproximadamente cada 1 seg
    xLastWakeTime = xTaskGetTickCount();
    Board_LED_Toggle(2); /* Blue (RGB) */
  }
}
/*-----------------------------------------------------------*/
