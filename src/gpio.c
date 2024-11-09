#include "gpio.h"
#include "debug.h"
#include <assert.h>
void assert_failed(char *file, int line)
{
    printk("assert failed: %s at %d\n", file, line);
}

#define assert_param(expr) ((expr) ? (void)0 : assert_failed(__FILE__, __LINE__))

void GPIO_ClockDisable(GPIO_TypeDef *pGPIOx)
{
}

void GPIO_ClockEnable(GPIO_TypeDef *pGPIOx)
{
    if (pGPIOx == GPIOA)
    {
        __RCC_GPIOA_CLK_EN();
    }
    else if (pGPIOx == GPIOB)
    {
        __RCC_GPIOB_CLK_EN();
    }
    else if (pGPIOx == GPIOC)
    {
        __RCC_GPIOC_CLK_EN();
    }
    else if (pGPIOx == GPIOD)
    {
        __RCC_GPIOD_CLK_EN();
    }
    else if (pGPIOx == GPIOE)
    {
        __RCC_GPIOE_CLK_EN();
    }
    else if (pGPIOx == GPIOE)
    {
        __RCC_GPIOH_CLK_EN();
    }
}

void GPIO_Init(GPIO_TypeDef *pGPIOx, GPIOConfig_t *pConfig)
{
    pGPIOx->MODER &= ~(3U << pConfig->Pin * 2);
    pGPIOx->MODER |= (pConfig->Mode << (pConfig->Pin * 2));

    if (pConfig->Mode != GPIO_MODE_INPUT)
    {
        pGPIOx->OTYPER &= ~(1U << pConfig->Pin);
        pGPIOx->OTYPER |= (pConfig->OType << pConfig->Pin);
    }
    if (pConfig->Mode != GPIO_MODE_OUTPUT)
    {
        pGPIOx->PUPDR &= (3U << (pConfig->Pin * 2));
    }

    pGPIOx->OSPEEDR &= ~(3U << pConfig->Pin * 2);
    pGPIOx->OSPEEDR |= (pConfig->OSpeed << (pConfig->Pin * 2));

    if (pConfig->Mode == GPIO_MODE_ALTFN)
    {
        pGPIOx->AFR[(uint32_t)(pConfig->Pin / 8)] |= pConfig->AFn << ((pConfig->Pin % 8) * 4U);
    }
}

void GPIO_WritePort(GPIO_TypeDef *pGPIO, uint32_t Hex)
{
}

void GPIO_WritePin(GPIO_TypeDef *pGPIOx, uint32_t Pin, GPIO_PinState PinState)
{
    if (PinState == GPIO_PIN_LOW)
    {
        pGPIOx->BSRR |= 1U << (Pin + 16u);
    }
    else if (PinState == GPIO_PIN_HIGH)
    {
        pGPIOx->BSRR |= 1U << Pin;
    }
}