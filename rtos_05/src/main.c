#include "stdio.h"

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/* LPCOpen includes*/
#include "board.h"

/* Demo includes. */
#include "supporting_functions.h"

/* macros and definitions */

#define TAM_COLA 10
#define TAM_PILA 1024


static xQueueHandle xQueue; /* Cola para recibir */

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
void vTask3( void *pvParameters );

/* Init Queue */
static void InitQueue(void);
/*-----------------------------------------------------------*/

int main( void )
{
  InitQueue();
   /* Create one of the two tasks. */
  xTaskCreate( vTask1, (const char *)"Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
   /* Create the other task in exactly the same way. */
  xTaskCreate( vTask2, (const char *)"Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
   /* Create the other task in exactly the same way. */
  xTaskCreate( vTask3, (const char *)"Task 3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );

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
  BaseType_t xStatus;
  const TickType_t xValueToSend = pdMS_TO_TICKS( 1000UL );

  for( ;; )
  {
    xStatus = xQueueSend( xQueue, &xValueToSend, 0 );

    if( xStatus != pdPASS )
    {
      /* The send operation could not complete because the queue was full -
      this must be an error as the queue should never contain more than
      one item! */
      vPrintString( "Could not send to the queue.\r\n" );
    }

    vPrintString( "vTask1 envie dato a la cola" );
  }
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
  BaseType_t xStatus;
  const TickType_t xValueToSend = pdMS_TO_TICKS( 500UL );

  for( ;; )
  {
    xStatus = xQueueSend( xQueue, &xValueToSend, 0 );

    if( xStatus != pdPASS )
    {
      /* The send operation could not complete because the queue was full -
      this must be an error as the queue should never contain more than
      one item! */
      vPrintString( "Could not send to the queue.\r\n" );
    }

    vPrintString( "vTask2 envie dato a la cola" );
  }
}
/*-----------------------------------------------------------*/

void vTask3( void *pvParameters )
{
  const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );
  TickType_t xTimeOn; //tiempo en ms

	while (1){
		if(xQueueReceive (xQueue , &xTimeOn , portMAX_DELAY) == pdTRUE){
      Board_LED_Set(3, TRUE); //enciende el led amarillo
      vPrintString( "vTask3 recibi dato de la cola" );
      vTaskDelay(xTimeOn);    //el tiempo enviado por las otras tareas
    }

    Board_LED_Set(3, FALSE); //apaga el led amarillo
    vTaskDelay(xDelay500ms); //500 ms
  }
}
/*-----------------------------------------------------------*/

static void InitQueue(void)
{
  /* Primero se crea la cola */
  xQueue = xQueueCreate ( TAM_COLA, sizeof (TickType_t) );

  if( xQueue == NULL ){
    Board_LED_Set( 5, TRUE ); /* prende led rojo indica error Fatal */
    while ( 1 ); /* Se queda bloqueado el sistema hasta que venga el técnico de mantenimiento */
  }
}
