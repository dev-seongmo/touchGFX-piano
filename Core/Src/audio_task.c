#include "audio_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"

extern TIM_HandleTypeDef htim4;

static SoundProfile currentProfile = SOUND_PROFILE_TWINKLE;
static QueueHandle_t audioInputQueue = NULL;

static const uint16_t twinklePscByLane[8] = {320, 285, 254, 239, 213, 190, 169, 159};
static const uint16_t tetrisPscByLane[8] = {304, 270, 255, 227, 202, 191, 170, 152};

void AudioTask_Init(void)
{
    if (audioInputQueue == NULL)
    {
        /* 최신 입력 하나만 알면 되니까 큐 길이는 1이면 충분하다. */
        audioInputQueue = xQueueCreate(1, sizeof(uint8_t));
    }
}

void StartAudioTask(void* argument)
{
    uint8_t mask = 0u;

    (void)argument;

    for (;;)
    {
        /* 오디오는 급하긴 한데, 할 일 없을 때까지 바쁘게 돌 필요는 없다. */
        if ((audioInputQueue != NULL) && (xQueueReceive(audioInputQueue, &mask, portMAX_DELAY) == pdPASS))
        {
            AudioTask_PlayMask(mask);
        }
    }
}

void AudioTask_PushMask(uint8_t mask)
{
    if (audioInputQueue == NULL)
    {
        return;
    }

    (void)xQueueOverwrite(audioInputQueue, &mask);
}

void AudioTask_SetProfile(SoundProfile profile)
{
    currentProfile = profile;
}

static uint16_t getLanePsc(uint8_t lane)
{
    if (lane < 1u || lane > 8u)
    {
        return 0u;
    }

    if (currentProfile == SOUND_PROFILE_TETRIS)
    {
        return tetrisPscByLane[lane - 1u];
    }

    return twinklePscByLane[lane - 1u];
}

static void playLane(uint8_t lane)
{
    const uint16_t psc = getLanePsc(lane);

    if (psc == 0u)
    {
        /* 누른 게 없으면 스피커도 쉬어야 한다. */
        TIM4->CCR2 = 0;
        return;
    }

    /* 음 하나 바꾼다고 타이머를 다 뒤엎을 건 아니고 필요한 값만 살짝 바꾼다. */
    TIM4->PSC = psc;
    TIM4->CCR2 = 500;
}

void AudioTask_PlayMask(uint8_t inputMask)
{
    uint8_t lane = 0;

    /* 여러 개 눌려도 지금은 제일 앞 lane 하나만 쓴다. 욕심내면 규칙부터 정해야 한다. */
    for (uint8_t i = 0; i < 8; i++)
    {
        if ((inputMask & (1u << i)) != 0u)
        {
            lane = (uint8_t)(i + 1u);
            break;
        }
    }

    playLane(lane);
}
