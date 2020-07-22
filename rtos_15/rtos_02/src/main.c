/*  Cree un programa usando FreeRtos que cumpla con las siguientes consignas:
*
*     - Posea dos tareas (Tarea1 y Tarea2) y una rutina de atención de
*     interrupción de timer RIT (Repetitive Interrupt Timer)
*
*     - La Tarea1 debe producir elementos.
*
*     - La Tarea2 debe esperar de la interrupción del timer RIT (que será de 3
*     segundos).
*
*     - Una vez que expira el timer, consumir un elemento, informar por puerto
*     serie el elemento consumido y cambiar el estado del led Amarillo.
*/


#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "supporting_functions.h"

#define TAM_COLA 100

static xQueueHandle xQueue;
xSemaphoreHandle xBinarySemaphore;
uint16_t sElement = 0;

static void vProducer( void *pvParameters );
static void vConsumer( void *pvParameters );
static void vInitQueue( void );
static void vInitHardware( void );
static void vInitTimer( void );
void RIT_IRQHandler( void );


int main(void)
{
  vInitHardware(); 
	vInitTimer();
  vInitQueue();

  vSemaphoreCreateBinary ( xBinarySemaphore );
  xSemaphoreTake( xBinarySemaphore, 0 );

  xTaskCreate( vProducer, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
  xTaskCreate( vConsumer, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );

  vTaskStartScheduler ();
}


static void vProducer ( void *pvParameters )
{
  BaseType_t xStatus;
  const TickType_t xDelay100ms = pdMS_TO_TICKS( 100UL );

  for( ;; )
  {
    sElement = sElement + 1;
    xStatus = xQueueSend( xQueue, &sElement, portMAX_DELAY );

    if( xStatus != pdPASS )
    {
      vPrintString( "Could not send to the queue.\r\n" );
    }

    printf( "Element %d has been produced\r\n", sElement );
    vTaskDelay( xDelay100ms );
  }
}

static void vConsumer(void *pvParameters)
{
    uint16_t sElementConsumed;

    for( ;; ) {
     xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
     if(xQueueReceive ( xQueue, &sElementConsumed, 0 ) == pdTRUE )
     {
       printf( "Element %d has been consumed", sElementConsumed );
       Board_LED_Toggle( 3 ); /* Yellow */
     }

  }
}

static void vInitQueue( void )
{
  xQueue = xQueueCreate ( TAM_COLA, sizeof( uint16_t ) );

  if( xQueue == NULL ){
    Board_LED_Set( 5, TRUE ); /* RED */
    for( ;; );
  }
}

static void vInitHardware(void)
{
    SystemCoreClockUpdate();
    Board_Init();
}

static void vInitTimer( void )
{
	Chip_RIT_Init( LPC_RITIMER );
	Chip_RIT_SetTimerInterval( LPC_RITIMER, 3000 );
}

void RIT_IRQHandler( void )
{
	portBASE_TYPE xTaskWoken = pdFALSE ;

	xTaskWoken = xSemaphoreGiveFromISR ( xBinarySemaphore, &xTaskWoken );

	Chip_RIT_ClearInt( LPC_RITIMER );
}
