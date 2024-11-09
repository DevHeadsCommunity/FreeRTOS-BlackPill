
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
/* Standard includes. */
#include <stdio.h>
#include "stm32f411xe.h"
#include "uart.h"
#include "debug.h"
#include "gpio.h"

/*-----------------------------------------------------------*/

const uint8_t led_one_pin = 7;
const uint8_t led_two_pin = 5;
const uint8_t led_three_pin = 3;

UARTConfig_t usart;

void UART2_Config();
void GPIO_Config();
void BlinkLedOneTask(void *params);
void BlinkLedTwoTask(void *params);
void BlinkLedThreeTask(void *params);

void uart_gpio_init()
{
    // Enabling Clock for GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configuring RX and TX Pin
    GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;
    GPIOA->OTYPER |= GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3;
    GPIOA->AFR[0] |= 7 << GPIO_AFRL_AFSEL2_Pos | 7 << GPIO_AFRL_AFSEL3_Pos;
}

int main(void)
{
    // UART2_GPIO_Init();
    uart_gpio_init();
    UART2_Config();
    GPIO_Config();

    if (xTaskCreate(BlinkLedOneTask,
                    "BlinkLedOneTask",
                    (configMINIMAL_STACK_SIZE), // Double the minimal stack size
                    NULL,
                    configMAX_PRIORITIES - 1U,
                    NULL) != pdPASS)
    {
        printk("Task creation failed!\n");
        for (;;)
            ;
    }
    else
    {
        printk("blink_led_one_task created sucsessfully.\n");
    }

    if (xTaskCreate(BlinkLedTwoTask,
                    "BlinkLedTwoTask",
                    (configMINIMAL_STACK_SIZE), // Double the minimal stack size
                    NULL,
                    configMAX_PRIORITIES - 1U,
                    NULL) != pdPASS)
    {
        printk("Task creation failed!\n");
        for (;;)
            ;
    }
    else
    {
        printk("blink_led_two_task created sucsessfully.\n");
    }

    if (xTaskCreate(BlinkLedThreeTask,
                    "BlinkLedThreeTask",
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    configMAX_PRIORITIES - 1U,
                    NULL) != pdPASS)
    {
        printk("Task Creation Failed\n");
        for (;;)
            ;
    }

    vTaskStartScheduler();

    for (;;)
    {
    }

    return 0;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* Print or log stack overflow for task debugging */
    printk("Stack overflow in task: %s\n", pcTaskName);
    for (;;)
        ; // Halt system to debug
}

void UART2_Config()
{
    usart.p_usart = USART2;
    usart.baud_rate = 115200U;
    usart.mode = TX_ONLY;
    usart.parity = NO_PARITY;
    usart.word_len = W8_BTIS;
    UART_Init(&usart);
}

void GPIO_Config()
{
    GPIO_ClockEnable(GPIOB);

    GPIOConfig_t led_one, led_two, led_three;

    led_one.Pin = led_one_pin;
    led_one.Mode = GPIO_MODE_OUTPUT;
    led_one.OType = GPIO_OTYPE_PP;
    GPIO_Init(GPIOB, &led_one);

    led_two.Pin = led_two_pin;
    led_two.Mode = GPIO_MODE_OUTPUT;
    led_two.OType = GPIO_OTYPE_PP;
    GPIO_Init(GPIOB, &led_two);

    led_three.Pin = led_three_pin;
    led_three.Mode = GPIO_MODE_OUTPUT;
    led_three.OType = GPIO_OTYPE_PP;
    GPIO_Init(GPIOB, &led_three);
}

void BlinkLedOneTask(void *params)
{
    (void)params; // Unused Task

    for (;;)
    {
        GPIO_WritePin(GPIOB, led_one_pin, GPIO_PIN_LOW);
        vTaskDelay(pdMS_TO_TICKS(1000));

        GPIO_WritePin(GPIOB, led_one_pin, GPIO_PIN_HIGH);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void BlinkLedTwoTask(void *params)
{
    (void)params; // Unused Task

    for (;;)
    {
        GPIO_WritePin(GPIOB, led_two_pin, GPIO_PIN_LOW);
        vTaskDelay(pdMS_TO_TICKS(500));

        GPIO_WritePin(GPIOB, led_two_pin, GPIO_PIN_HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
void BlinkLedThreeTask(void *params)
{
    (void)params;

    for (;;)
    {
        GPIO_WritePin(GPIOB, led_three_pin, GPIO_PIN_LOW);
        vTaskDelay(pdMS_TO_TICKS(500));

        GPIO_WritePin(GPIOB, led_three_pin, GPIO_PIN_HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}