/* Reescriba el código mostrado en la sección 4.5.6 (página 105) del libro
* Sistemas Empotrados en Tiempo Real usando el sistema operativo FreeRTOS. Use
* para todos los semáforos un timeout de 1000. Describa la ejecución de ambas
* tareas suponiendo que inicialmente ambos semáforos están libres, que
* inicialmente se ejecuta la Tarea1 y que se produce un cambio de contexto justo
* antes de que la Tarea1 pida el semáforo B. Ilustre esta descripción mediante
* un diagrama similar al mostrado en la figura 4.4.
*/

 /*==================[inclusions]=============================================*/

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "supporting_functions.h"


xSemaphoreHandle xBinarySemaphoreA;
xSemaphoreHandle xBinarySemaphoreB;

static void vTask1 ( void *pvParameters );
static void vTask2 ( void *pvParameters );
static void vFatalError( void *pvParameters );

int main(void)
{
  vSemaphoreCreateBinary ( xBinarySemaphoreA );
  vSemaphoreCreateBinary ( xBinarySemaphoreB );

  if( xBinarySemaphoreA != NULL && xBinarySemaphoreB != NULL )
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
   TickType_t xLastWakeTime = 0;
   const TickType_t xTimeOut = pdMS_TO_TICKS( 1000UL );

   for( ;; )
   {
     xSemaphoreTake( xBinarySemaphoreA, xTimeOut );
     xSemaphoreTake( xBinarySemaphoreB, xTimeOut );
     printf( "Running Task1. Delta Ticks = %d\r\n", xTaskGetTickCount() - xLastWakeTime );
     xLastWakeTime = xTaskGetTickCount();
     xSemaphoreGive( xBinarySemaphoreB );
     xSemaphoreGive( xBinarySemaphoreA );
   }
}

static void vTask2( void *pvParameters )
{
  TickType_t xLastWakeTime = 0;
  const TickType_t xTimeOut = pdMS_TO_TICKS( 1000UL );

  for( ;; )
  {
    xSemaphoreTake( xBinarySemaphoreB, xTimeOut );
    xSemaphoreTake( xBinarySemaphoreA, xTimeOut );
    printf( "Running Task2. Delta Ticks = %d\r\n", xTaskGetTickCount() - xLastWakeTime );
    xLastWakeTime = xTaskGetTickCount();
    xSemaphoreGive( xBinarySemaphoreA );
    xSemaphoreGive( xBinarySemaphoreB );
  }
}

static void vFatalError( void *pvParameters )
{
  const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000UL );

  for( ;; )
  {
    vPrintString( "Fatal Error. Semaphore coudn't be created\r\n" );
    Board_LED_Toggle(4); /* Red */
    vTaskDelay( xDelay1000ms );
  }
}

/* Al ejercutarse la Task1 se intenta tomar el Semáforo A, como está
libre, la llamada retornará inmediatamente. En este momento se produce un cambio
de contexto antes que la Task1 pida el Semáforo B. Este cambio de contexto hace
que comience a ejecutarse la Task2. Esta pedirá primero el Semáforo B, el cual
al estar libre se le dará. A continuación pedirá el Semáforo B, pero como está
tomado por la Task1, la Tarea2 se bloqueará a la espera de que dicho semáforo
quede libre. Se producirá ahora un nuevo cambio de contexto y se ejecutará la
Task1 desde donde se dejó, pedirá el semáforo B y se bloqueará ya que está
tomado por la Task2.Como ambas tareas están bloqueadas se ejecuta la IDLE.

El primer Timeout que expira es el del Semáforo A en la Task1. Cuando esto
sucede, se libera y por lo tanto es tomado por la Task2 que ahora continúa
ejecutandose, imprime un mensaje por el puerto serie y libera ambos semáforos.

Si los semáforos no tuvieran un timeout finito como en este caso, se produce
lo que se conoce como Deadlock. La forma optima de evitar el Deadlock es
programar lo semáforos para que se pidan siempre los semáforos en el mismo
orden en todas las tareas.
*/
