#ifndef INPUT_TASK_H
#define INPUT_TASK_H

#include <stdint.h>

void InputTask_Init(void);
void StartInputTask(void* argument);
void InputTask_SetTask(void);
uint32_t InputTask_Wait(uint32_t timeoutTicks);
void InputTask_OnInterrupt(uint16_t gpioPin);
uint8_t InputTask_GetMask(void);

#endif /* INPUT_TASK_H */
