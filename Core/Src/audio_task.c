#include "audio_task.h"

#include "main.h"

extern TIM_HandleTypeDef htim4;

static SoundProfile currentProfile = SOUND_PROFILE_TWINKLE;

static const uint16_t twinklePscByLane[8] = {383, 340, 304, 287, 255, 227, 202, 191};
static const uint16_t tetrisPscByLane[8] = {304, 270, 255, 227, 202, 191, 170, 152};

void AudioTask_SetSoundProfile(SoundProfile profile)
{
    currentProfile = profile;
}

static uint16_t getPscForLane(uint8_t lane)
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
    const uint16_t psc = getPscForLane(lane);

    if (psc == 0u)
    {
        TIM4->CCR2 = 0;
        return;
    }

    TIM4->PSC = psc;
    TIM4->CCR2 = 500;
}

void AudioTask_PlayInputMask(uint8_t inputMask)
{
    uint8_t lane = 0;

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
