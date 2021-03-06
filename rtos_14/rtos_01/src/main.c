/* Ejericio 1 - Trabajo Practico 14 - FreeRTOS*/

/* Cree un programa usando FreeRtos que cumpla con las siguientes consignas:
- Posea dos tareas, Tarea1 y Tarea2, con los niveles de prioridad correspondientes.
- La Tarea1 debe ejecutarse de manera contínua, indicando este estado a través del
puerto serie.
- La Tarea2 debe ser periódica, con período de 1 segundo y debe cambiar el estado del
led verde e indicarlo a través del puerto serie. Adicionalmente debe indicar los ticks del
reloj de FreeRtos para asegurarse el período solicitado. */

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

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/*-----------------------------------------------------------*/

int main( void )
{
   /* Create one of the two tasks. */
   xTaskCreate( vTask1, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);

   /* Create the other task in exactly the same way. */
   xTaskCreate( vTask2, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );

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

   /* As per most tasks, this task is implemented in an infinite loop. */
   for( ;; ) {
      /* Print out the name of this task. */
      vPrintString( pcTaskName );
      }
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
  TickType_t xLastWakeTime;
  const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000UL ); //1 seg

  xLastWakeTime = xTaskGetTickCount(); /* se inicializa la variable con la actual cantidad de ticks. Luego es manejada por la API de vTaskDelayUntil()*/

  for( ;; ) {
     printf("Ticks = %d\r\n", xLastWakeTime);
     Board_LED_Toggle(5); //titila "LED 3" ( verde )
     /* pasa a READY cada 1 seg. */
     vTaskDelayUntil( &xLastWakeTime, xDelay1000ms );
  }
}
