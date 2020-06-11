/* Cree un programa usando FreeRtos que cumpla con las siguientes consignas:
* - Posea dos tareas, Tarea1 y Tarea2, con los niveles de prioridad correspondientes.
* - La Tarea1 debe ejecutarse de manera contínua, indicando este estado a través del puerto serie.
* - La Tarea2 debe ser periódica, con período de 1 segundo y debe cambiar el estado del led verde e indicarlo a través del puerto serie.
* Adicionalmente debe indicar los ticks del reloj de FreeRtos para asegurarse el período solicitado.
*  */
 /*==================[inclusions]=============================================*/

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

/*==================[macros and definitions]=================================*/

//#define PRIO_TAREA1 1
//#define PRIO_TAREA2 2
//#define TAM_PILA 150
//#define mainDELAY_LOOP_COUNT        ( 0xffffff )

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void vTarea1(void *pvParameters)
{
   /* As per most tasks, this task is implemented in an infinite loop. */
   for( ;; ) {
      vPrintString( "Tarea1 running\r\n" );
   }
}

static void vTarea2(void *pvParameters)
{
  TickType_t xLastWakeTime;
  const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000UL ); //1 seg

  xLastWakeTime = xTaskGetTickCount(); /* se inicializa la variable con la
  actual cantidad de ticks. Luego es manejada por la API de vTaskDelayUntil()*/

  for( ;; ) {
     print( "Ticks = %d\r\n", xLastWakeTime);
     Board_LED_Toggle(5); //titila "LED 3" ( verde )

     /* pasa a READY cada 1 seg. */
     vTaskDelayUntil( &xLastWakeTime, xDelay1000ms );
  }
}

/*==================[external functions definition]==========================*/

int main(void)
{
    //Se inicializa HW
	/* Se crean las tareas */
	xTaskCreate(vTarea1, (const char *)"Tarea1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate(vTarea2, (const char *)"Tarea2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );

	vTaskStartScheduler(); /* y por último se arranca el planificador . */
    //Nunca llegara a ese lazo  .... espero
     for( ;; );
     return 0;

}

/*==================[end of file]============================================*/
