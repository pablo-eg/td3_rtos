#include "stdio.h"

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* LPCOpen includes*/
#include "board.h"

/* Demo includes. */
#include "supporting_functions.h"

/* Used as a loop counter to create a very crude delay. */
//#define mainDELAY_LOOP_COUNT            ( 0xffffff )

/* Task2 handle */
TaskHandle_t xTask2Handle = NULL;

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/*-----------------------------------------------------------*/

int main( void )
{
   /* Create one of the two tasks. */
   xTaskCreate( vTask1, (const char *)"Task 1", 1000, NULL, tskIDLE_PRIORITY+2, NULL );
   /* Create the other task in exactly the same way. */
   xTaskCreate( vTask2, (const char *)"Task 2", 1000, NULL, tskIDLE_PRIORITY+1, xTask2Handle);

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
   const char *pcTaskName = "Task 1 is running\r\n";
   TickType_t xLastWakeTime;
   const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000UL ); //1 seg

   xLastWakeTime = xTaskGetTickCount(); /* se inicializa la variable con la actual cantidad de ticks. Luego es manejada por la API de vTaskDelayUntil()*/

   /* As per most tasks, this task is implemented in an infinite loop. */
   for( ;; ){
      /* Print out the name of this task. */
      vPrintString( pcTaskName );
      if (xLastWakeTime - 100 >= 0){ //aproximadamente cada 1 seg
        xLastWakeTime = xTaskGetTickCount();
        vTaskPrioritySet( xTask2Handle, tskIDLE_PRIORITY + 3 ); //aumenta la prioriodad de la tarea 2 por encima de la tarea 1
      }
    }
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
   const char *pcTaskName = "Task 2 is running\r\n";

   /* As per most tasks, this task is implemented in an infinite loop. */
   for( ;; ) {
      /* Print out the name of this task. */
      vPrintString( pcTaskName );
      Board_LED_Toggle(5);
      vTaskPrioritySet( NULL, tskIDLE_PRIORITY + 1 ); //disminuye su propia prioridad
   }
}
