/* Ejericio 4 - Trabajo Practico 14 - FreeRTOS*/

/*Cree un programa usando FreeRtos que cumpla con las siguientes consignas:
- Posea dos tareas, Tarea1 y Tarea2 de distinta prioridad y que sean
implementadas en la misma función.
- Ambas tareas deben indicar por puerto serie que están en ejecución y su
prioridad, luego bloquearse por 500ms.
- En el tiempo Idle aprovechar para cambiar el estado del led Azul cada 300ms e
indicarlo por puerto serie. (Idle Hook)
- Justifique que sucede con los cambios de estado del led en caso que la tarea
Tarea1 pase a ejecutarse de manera contínua. */

/*IMPORTANTE: configUSE_IDLE_HOOK must be set to 1 in FreeRTOSConfig.h for the idle hook
function to get called */

#include "stdio.h"

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* LPCOpen includes*/
#include "board.h"

/* Demo includes. */
#include "supporting_functions.h"

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
  const char *pcTaskName = "Task 1 is running. Priority ";
  const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL ); //500 ms
  UBaseType_t uxPriority;
  char *pcTaskPriority;

  uxPriority = uxTaskPriorityGet( NULL );
  //pcTaskPriority = (char *) uxPriority;

  /* As per most tasks, this task is implemented in an infinite loop. */
  for( ;; ) {
     /* Print out the name of this task. */
     vPrintString( pcTaskName );
     printf( "%d \r\n", uxPriority );

     vTaskDelay( xDelay500ms );
  }
}
/*-----------------------------------------------------------*/


void vTask2( void *pvParameters )
{
   const char *pcTaskName = "Task 2 is running. Priority ";
   const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL ); //500 ms
   UBaseType_t uxPriority;
   char *pcTaskPriority;

   uxPriority = uxTaskPriorityGet( NULL );
   //pcTaskPriority = (char *) uxPriority;

   /* As per most tasks, this task is implemented in an infinite loop. */
   for( ;; ) {
      /* Print out the name of this task. */
      vPrintString( pcTaskName );
      printf( "%d \r\n", uxPriority );

      vTaskDelay( xDelay500ms );
   }
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xDelay300ms = 30;

  for(;;){
    if (xTaskGetTickCount() - xLastWakeTime >= xDelay300ms){
      xLastWakeTime = xTaskGetTickCount();
      Board_LED_Toggle(2); /* Blue (RGB) */
    }
  }
}
/*-----------------------------------------------------------*/


/* Justifique que sucede con los cambios de estado del led en caso que la tarea
Tarea1 pase a ejecutarse de manera contínua.

Si se comenta la línea 74, la tarea 1 pasa a ejecutarse de manera continua.
Como la prioridad de la tarea 1 es mayor que la de la idle hook, veremos que el
led nunca parpadea y por el puerto serie solo veremos el mensaje
"Task 1 is running. Priority 2", ya que la prioridad de la tarea 1 es mayor que
la de la 2. Al ejecutar la tarea 1 de manera continua lo correcto hubiese sido
poner una prioridad mayor a la tarea 2.
*/
