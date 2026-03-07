#include "rhythm_game.h"

#include "audio_task.h"
#include "cmsis_os.h"
#include "input_task.h"

static volatile RhythmSharedState sharedState = { 0u, 0u, 0u };
static uint32_t songStartTick = 0;

static void updateSongClock(void)
{
    if (sharedState.playing != 0u)
    {
        const uint32_t nowTick = osKernelGetTickCount();
        const uint32_t tickHz = osKernelGetTickFreq();
        const uint32_t dtTick = nowTick - songStartTick;

        sharedState.song_ms = (dtTick * 1000u) / tickHz;
    }
    else
    {
        songStartTick = osKernelGetTickCount();
        sharedState.song_ms = 0u;
    }
}

void RhythmGame_Init(void)
{
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
    AudioTask_PlayInputMask(0u);
}

void RhythmGame_SetSoundProfile(RhythmSoundProfile profile)
{
    AudioTask_SetSoundProfile((profile == RHYTHM_SOUND_TETRIS) ? SOUND_PROFILE_TETRIS : SOUND_PROFILE_TWINKLE);
}

void RhythmGame_GetState(RhythmSharedState* state)
{
    if (state == 0)
    {
        return;
    }

    state->song_ms = sharedState.song_ms;
    state->playing = sharedState.playing;
    state->input_mask = sharedState.input_mask;
}

void StartEngineTask(void* argument)
{
    (void)argument;

    for (;;)
    {
        const uint8_t inputMask = InputTask_ReadMask();

        updateSongClock();
        sharedState.input_mask = inputMask;
        AudioTask_PlayInputMask(inputMask);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
