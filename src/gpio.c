#include "gpio.h"

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