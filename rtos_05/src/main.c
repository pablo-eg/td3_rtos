/* Ejericio 5 - Trabajo Practico 14 - FreeRTOS*/

/* Cree un programa usando FreeRtos que cumpla con las siguientes consignas:
- Posea las tareas Tarea1, Tarea2 y Tarea3.
- Las Tareas Tarea1 y Tarea2 deben enviar por cola de mensaje a la Tarea3 el
tiempo en milisegundos que debe estar encendido el led amarillo e indicarlo por
puerto serie.
- La Tarea3 debe leer esos tiempos de la cola a medida que llegan y controlar el
led en cuestión. Siempre, el tiempo apagado del led es de 500ms. También debe
indicar por puerto serie sus acciones.
- Indique que pasa en caso de que el timeout de la escritura en la cola de
mensajes sea de 300ms. */

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
    xStatus = xQueueSend( xQueue, &xValueToSend, portMAX_DELAY );

    if( xStatus != pdPASS )
    {
      /* The send operation could not complete because the queue was full -
      this must be an error as the queue should never contain more than
      one item! */
      vPrintString( "Could not send to the queue.\r\n" );
    }

    vPrintString( "vTask1 envie dato a la cola\r\n" );
  }
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
  BaseType_t xStatus;
  const TickType_t xValueToSend = pdMS_TO_TICKS( 500UL );

  for( ;; )
  {
    xStatus = xQueueSend( xQueue, &xValueToSend, portMAX_DELAY );

    if( xStatus != pdPASS )
    {
      /* The send operation could not complete because the queue was full -
      this must be an error as the queue should never contain more than
      one item! */
      vPrintString( "Could not send to the queue.\r\n" );
    }

    vPrintString( "vTask2 envie dato a la cola\r\r" );
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
      vPrintString( "vTask3 recibi dato de la cola\r\n" );
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


/* Indique que pasa en caso de que el timeout de la escritura en la cola de
mensajes sea de 300ms.

Como la cola se llena rapidamente en los primeros milisegundos de ejecución y la
tarea 3, de mayor prioridad, lee de la cola en un tiempo mayor a 300 ms (ya que
siempre se bloquea por 500 ms o 1000 ms), xStatus, que recibe lo que devuelve
xQueueSend, toma periodicamente el valor pdFALSE, por lo que veremos por el
puerto serie "Could not send to the queue." hasta que la tarea 3 lea de la cola.
Luego la tarea 1 o 2 escriben en la cola, se llena y el proceso se vuelve a
repetir.
*/
