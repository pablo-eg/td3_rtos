/* Cree un programa usando FreeRtos que cumpla con las siguientes consignas:
*
*     - Posea dos tareas, Tarea1 y Tarea2 con los niveles de prioridad
*       correspondientes y un mecanismo de sincronización.
*
*     - La Tarea1, debe ejecutar de manera continua un bucle de busy waiting de
*       1 segundo aproximadamente, indicándo este estado a través del puerto serie.
*
*     - La Tarea2 debe estar serializada, y esperar por cada loop de Tarea1,
*       cambiando el estado del led Azul, cada vez que Tarea1 complete el loop.
*
*   Justifique que herramienta de sincronización utilizó
*/

 /*==================[inclusions]=============================================*/

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "supporting_functions.h"


xSemaphoreHandle xBinarySemaphore;

static void vTask1 ( void *pvParameters );
static void vTask2 ( void *pvParameters );
static void vFatalError ( void *pvParameters );

int main(void)
{
  /* Before a semaphore is used it must be explicitly created. In this example
  a binary semaphore is created. */
  vSemaphoreCreateBinary ( xBinarySemaphore );

  /* Check the semaphore was created successfully. */
  if( xBinarySemaphore != NULL )
  {
    xTaskCreate( vTask1, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
  	xTaskCreate( vTask2, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );

  	vTaskStartScheduler ();
  }

  /* Fatal Error. Semaphore coudn't be created */
  xTaskCreate( vFatalError, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );

  vTaskStartScheduler();

}


static void vTask1 ( void *pvParameters )
{
   const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000UL );

   for( ;; ) {
      printf( "vTask1 running\r\n" );
      vTaskDelay( xDelay1000ms );
      xSemaphoreGive( xBinarySemaphore );
   }
}

static void vTask2(void *pvParameters)
{
    for( ;; ) {
     xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
     Board_LED_Toggle(2); /* Blue (RGB) */
     printf( "vTask1 running" );
  }
}

static void vFatalError(void *pvParameters)
{
  const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000UL );

  for( ;; )
  {
    vPrintString( "Fatal Error. Semaphore coudn't be created\r\n" );
    Board_LED_Toggle(4); /* Red */
    vTaskDelay( xDelay1000ms );
  }
}
