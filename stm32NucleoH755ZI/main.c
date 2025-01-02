


/*

chatgpt 3.5

This example creates two tasks, one for each core. Core 1 Task continuously gives a semaphore, and Core 2 Task waits for the semaphore. When Core 2 Task receives the semaphore, it performs its task and then delays for 1 second. Meanwhile, Core 1 Task also delays for 1 second before giving the semaphore again.

Remember that when working with multicore systems, it's essential to synchronize data accesses and ensure mutual exclusion when necessary. Semaphores, mutexes, and other synchronization primitives provided by FreeRTOS can help achieve this.

*/



/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Standard includes */
#include <stdio.h>

/* STM32Cube HAL includes */
#include "stm32h7xx_hal.h"

/* Task function prototypes */
void vCore1Task(void *pvParameters);
void vCore2Task(void *pvParameters);

/* Semaphore handle */
SemaphoreHandle_t xSemaphore = NULL;

int main(void)
{
    /* Initialize HAL */
    HAL_Init();

    /* Initialize FreeRTOS */
    vTaskStartScheduler();

    /* We should never get here as control is now taken by the scheduler */
    while(1);
}

void vCore1Task(void *pvParameters)
{
    for(;;)
    {
        printf("Core 1 Task running\r\n");

        /* Give the semaphore to Core 2 Task */
        xSemaphoreGive(xSemaphore);

        /* Delay for 1 second */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vCore2Task(void *pvParameters)
{
    for(;;)
    {
        /* Wait for the semaphore */
        if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            printf("Core 2 Task running\r\n");

            /* Perform some task */

            /* Delay for 1 second */
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}


