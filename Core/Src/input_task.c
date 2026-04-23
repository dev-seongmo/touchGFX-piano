#include "input_task.h"

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rhythm_game.h"

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

typedef struct
{
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t bit;
} InputPin;

static const InputPin inputPins[] = {
    { DO_INPUT_GPIO_PORT, DO_INPUT_PIN, 0u },
    { RE_INPUT_GPIO_PORT, RE_INPUT_PIN, 1u },
    { MI_INPUT_GPIO_PORT, MI_INPUT_PIN, 2u },
    { FA_INPUT_GPIO_PORT, FA_INPUT_PIN, 3u },
    { SOL_INPUT_GPIO_PORT, SOL_INPUT_PIN, 4u },
    { LA_INPUT_GPIO_PORT, LA_INPUT_PIN, 5u },
    { SI_INPUT_GPIO_PORT, SI_INPUT_PIN, 6u },
    { DOO_INPUT_GPIO_PORT, DOO_INPUT_PIN, 7u }
};

static volatile uint8_t cachedInputMask = 0u;
static TaskHandle_t inputListenerTaskHandle = NULL;

static uint8_t readInputs(void)
{
    uint8_t mask = 0u;

    /* 부팅 직후 상태도 챙겨둬야 첫 입력에서 안 어색하다. */
    for (uint32_t i = 0; i < (sizeof(inputPins) / sizeof(inputPins[0])); i++)
    {
        if (HAL_GPIO_ReadPin(inputPins[i].port, inputPins[i].pin) == GPIO_PIN_SET)
        {
            mask |= (uint8_t)(1u << inputPins[i].bit);
        }
    }

    return mask;
}

void InputTask_Init(void)
{
    cachedInputMask = readInputs();
}

void StartInputTask(void* argument)
{
    (void)argument;

    /* ISR에서 깨워줄 대상을 먼저 등록해둔다. 안 그러면 한참 기다린다. */
    InputTask_SetTask();
    RhythmGame_PushInput(InputTask_GetMask());

    for (;;)
    {
        /* 여기서 폴링으로 돌리기 시작하면 또 원점회귀라 그냥 얌전히 기다린다. */
        (void)InputTask_Wait(portMAX_DELAY);
        RhythmGame_PushInput(InputTask_GetMask());
    }
}

void InputTask_SetTask(void)
{
    taskENTER_CRITICAL();
    inputListenerTaskHandle = xTaskGetCurrentTaskHandle();
    taskEXIT_CRITICAL();
}

uint32_t InputTask_Wait(uint32_t timeoutTicks)
{
    return ulTaskNotifyTake(pdTRUE, timeoutTicks);
}

void InputTask_OnInterrupt(uint16_t gpioPin)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    uint8_t mask = (uint8_t)cachedInputMask;
    uint8_t found = 0u;

    /* EXTI 라인은 왔는데 우리가 쓰는 핀이 아닐 수도 있어서 한번 걸러준다. */
    for (uint32_t i = 0; i < (sizeof(inputPins) / sizeof(inputPins[0])); i++)
    {
        if (inputPins[i].pin != gpioPin)
        {
            continue;
        }

        found = 1u;

        if (HAL_GPIO_ReadPin(inputPins[i].port, inputPins[i].pin) == GPIO_PIN_SET)
        {
            mask |= (uint8_t)(1u << inputPins[i].bit);
        }
        else
        {
            mask &= (uint8_t)~(1u << inputPins[i].bit);
        }
    }

    if (found == 0u)
    {
        return;
    }

    cachedInputMask = mask;

    if (inputListenerTaskHandle == NULL)
    {
        /* 아직 태스크가 안 떴으면 캐시만 갱신하고 조용히 빠진다. */
        return;
    }

    /* 입력 들어왔으니 엔진 쪽으로 공 던져준다. */
    vTaskNotifyGiveFromISR(inputListenerTaskHandle, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

uint8_t InputTask_GetMask(void)
{
    return cachedInputMask;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    InputTask_OnInterrupt(GPIO_Pin);
}
