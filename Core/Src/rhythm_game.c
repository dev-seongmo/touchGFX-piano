#include "rhythm_game.h"

#include "audio_task.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "queue.h"

static volatile RhythmSharedState sharedState = { 0u, 0u, 0u };
static uint32_t songStartTick = 0;
static QueueHandle_t engineInputQueue = NULL;

static void updateSongTime(void)
{
    if (sharedState.playing != 0u)
    {
        const uint32_t nowTick = osKernelGetTickCount();
        const uint32_t tickHz = osKernelGetTickFreq();
        const uint32_t dtTick = nowTick - songStartTick;

        /* tick을 바로 믿으면 사람이 못 읽으니 ms로 바꿔서 들고 간다. */
        sharedState.song_ms = (dtTick * 1000u) / tickHz;
    }
    else
    {
        /* 멈춘 상태에선 다음 시작을 위해 기준점만 다시 잡는다. */
        songStartTick = osKernelGetTickCount();
        sharedState.song_ms = 0u;
    }
}

void RhythmGame_Init(void)
{
    if (engineInputQueue == NULL)
    {
        /* 엔진도 최신 입력 하나면 충분하다. 지난 입력까지 줄 세울 필요는 없다. */
        engineInputQueue = xQueueCreate(1, sizeof(uint8_t));
    }

    sharedState.song_ms = 0u;
    sharedState.playing = 0u;
    sharedState.input_mask = 0u;
    songStartTick = 0u;
}

void RhythmGame_StartSong(void)
{
    songStartTick = osKernelGetTickCount();
    sharedState.song_ms = 0u;
    sharedState.playing = 1u;
}

void RhythmGame_StopSong(void)
{
    sharedState.song_ms = 0u;
    sharedState.playing = 0u;
    sharedState.input_mask = 0u;
    /* 정지했는데 소리 남아 있으면 제일 먼저 이상하다고 느낀다. */
    AudioTask_PushMask(0u);
}

void RhythmGame_SetProfile(RhythmSoundProfile profile)
{
    AudioTask_SetProfile((profile == RHYTHM_SOUND_TETRIS) ? SOUND_PROFILE_TETRIS : SOUND_PROFILE_TWINKLE);
}

void RhythmGame_GetSharedState(RhythmSharedState* state)
{
    if (state == 0)
    {
        return;
    }

    state->song_ms = sharedState.song_ms;
    state->playing = sharedState.playing;
    state->input_mask = sharedState.input_mask;
}

void RhythmGame_PushInput(uint8_t mask)
{
    if (engineInputQueue == NULL)
    {
        return;
    }

    (void)xQueueOverwrite(engineInputQueue, &mask);
}

void StartEngineTask(void* argument)
{
    (void)argument;
    uint8_t mask = 0u;

    for (;;)
    {
        /* 입력이 오면 바로 처리하고, 안 와도 시간은 흘러가니까 너무 오래 자진 않는다. */
        if ((engineInputQueue != NULL) && (xQueueReceive(engineInputQueue, &mask, pdMS_TO_TICKS(10)) == pdPASS))
        {
            sharedState.input_mask = mask;
            AudioTask_PushMask(mask);
        }

        /* 이 타이밍이 사실상 게임의 심장 박동 같은 셈이다. */
        updateSongTime();
    }
}
