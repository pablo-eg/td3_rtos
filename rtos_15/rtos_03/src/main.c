/*  Cree un programa usando FreeRtos que cumpla con las siguientes consignas:
*
*      - Posea tres tareas: Tarea1, Tarea2 y Tarea3.
*
*      - La Tarea1, debe tener la mayor prioridad, la Tarea2 media prioridad y
*      Tarea3 baja prioridad.
*
*      - La Tarea1 y la Tarea3 comparten el puerto serie, enviando un mensaje
*      cada una periodicamente, cada 800 milisegundos.
*
*      - La Tarea2 es una tarea periodica, que ejecuta repetidamente lo
*      siguiente: un bucle de busy waiting de 1 segundo aproximadamente, un
*      cambio de estado del led Rojo y un delay de 200 milisegundos.
*/


#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "supporting_functions.h"

#define mainDELAY_LOOP_COUNT  ( 0xaaaaaa )

SemaphoreHandle_t xMutex;

static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );
static void vTask3( void *pvParameters );

int main(void)
{
  xMutex = xSemaphoreCreateMutex();

  xTaskCreate( vTask1, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );
  xTaskCreate( vTask2, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
  xTaskCreate( vTask3, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );

  vTaskStartScheduler ();
}


static void vTask1 ( void *pvParameters )
{
  const TickType_t xDelay800ms = pdMS_TO_TICKS( 800UL );

  for( ;; )
  {
    xSemaphoreTake( xMutex, portMAX_DELAY );
    printf( "Top priority Task1 running           |\r\n" );
    xSemaphoreGive( xMutex );
    vTaskDelay( xDelay800ms );
  }
}

static void vTask2( void *pvParameters )
{
  uint32_t ul, ul2;
  const TickType_t xDelay200ms = pdMS_TO_TICKS( 200UL );

    for( ;; )
    {
      for( ul2 = 0; ul2 < 2; ul2++ )
      {
      	for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ ) //aprox 800 msx
      	{

        }
      }
      vPrintString( "Medium priority vTask2 running     |\r\n" );
      Board_LED_Toggle(4); /* Red */
      vTaskDelay( xDelay200ms );
    }
}

static void vTask3( void *pvParameters )
{
  const TickType_t xDelay800ms = pdMS_TO_TICKS( 800UL );

  for( ;; )
  {
    xSemaphoreTake( xMutex, portMAX_DELAY );
    printf( "Lowest priority vTask3 running  |\r\n" );
    xSemaphoreGive( xMutex );
    vTaskDelay( xDelay800ms );
  }
}
