/* Ejericio 2 - Trabajo Practico 14 - FreeRTOS*/

/* Cree un programa usando FreeRtos que cumpla con las siguientes consignas:
- Posea la tarea Tarea1.
- La Tarea1 debe crear la Tarea2 cada 500 ms, indicándolo por puerto serie.
- La Tarea2 debe cambiar el estado del led amarillo, indicarlo por puerto serie y
terminar. */

/* IMPORTANTE: INCLUDE_vTaskDelete debe estar seteado en 1 FreeRTOSConfig.h.
Por defecto se encuentra en 1. */


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
  /* Create the first task at priority 1. The task parameter is not used
  so is set to NULL. The task handle is also not used so likewise is set
  to NULL. */
  xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
  /* The task is created at priority 1 _______________________^. */
  /* Start the scheduler so the task starts executing. */
  vTaskStartScheduler();
  /* main() should never reach here as the scheduler has been started. */
  for( ;; );
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
  const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );

  for( ;; )
  {
  vPrintString( "Creating vTask2 from vTask1\r\n" );
  /* Create task 2 at a higher priority. Again the task parameter is not
  used so is set to NULL - BUT this time the task handle is required so
  the address of xTask2Handle is passed as the last parameter. */
  xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, &xTask2Handle );
  /* The task handle is the last parameter ______________________^^^^^^^^^^^^ */
  /* Task 2 has/had the higher priority, so for Task 1 to reach here Task 2
  must have already executed and deleted itself. Delay for 500 milliseconds. */
  vTaskDelay( xDelay500ms );
  }
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
  /* Task 2 does nothing but delete itself. To do this it could call vTaskDelete()
  using NULL as the parameter, but instead, and purely for demonstration purposes,
  it calls vTaskDelete() passing its own task handle. */
  vPrintString( "vTask2 is running and about to delete itself\r\n" );
  Board_LED_Toggle(3); /* Yellow */
  vTaskDelete( xTask2Handle );
}
