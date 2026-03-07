#include "input_task.h"

#include "main.h"

#define DO_INPUT_PIN GPIO_PIN_10
#define DO_INPUT_GPIO_PORT GPIOC
#define RE_INPUT_PIN GPIO_PIN_11
#define RE_INPUT_GPIO_PORT GPIOC
#define MI_INPUT_PIN GPIO_PIN_12
#define MI_INPUT_GPIO_PORT GPIOC
#define FA_INPUT_PIN GPIO_PIN_2
#define FA_INPUT_GPIO_PORT GPIOD
#define SOL_INPUT_PIN GPIO_PIN_4
#define SOL_INPUT_GPIO_PORT GPIOD
#define LA_INPUT_PIN GPIO_PIN_5
#define LA_INPUT_GPIO_PORT GPIOD
#define SI_INPUT_PIN GPIO_PIN_6
#define SI_INPUT_GPIO_PORT GPIOD
#define DOO_INPUT_PIN GPIO_PIN_7
#define DOO_INPUT_GPIO_PORT GPIOD

uint8_t InputTask_ReadMask(void)
{
    uint8_t inputMask = 0;

    if (HAL_GPIO_ReadPin(DO_INPUT_GPIO_PORT, DO_INPUT_PIN))
    {
        inputMask |= (1u << 0);
    }
    if (HAL_GPIO_ReadPin(RE_INPUT_GPIO_PORT, RE_INPUT_PIN))
    {
        inputMask |= (1u << 1);
    }
    if (HAL_GPIO_ReadPin(MI_INPUT_GPIO_PORT, MI_INPUT_PIN))
    {
        inputMask |= (1u << 2);
    }
    if (HAL_GPIO_ReadPin(FA_INPUT_GPIO_PORT, FA_INPUT_PIN))
    {
        inputMask |= (1u << 3);
    }
    if (HAL_GPIO_ReadPin(SOL_INPUT_GPIO_PORT, SOL_INPUT_PIN))
    {
        inputMask |= (1u << 4);
    }
    if (HAL_GPIO_ReadPin(LA_INPUT_GPIO_PORT, LA_INPUT_PIN))
    {
        inputMask |= (1u << 5);
    }
    if (HAL_GPIO_ReadPin(SI_INPUT_GPIO_PORT, SI_INPUT_PIN))
    {
        inputMask |= (1u << 6);
    }
    if (HAL_GPIO_ReadPin(DOO_INPUT_GPIO_PORT, DOO_INPUT_PIN))
    {
        inputMask |= (1u << 7);
    }

    return inputMask;
}
