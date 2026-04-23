#ifndef AUDIO_TASK_H
#define AUDIO_TASK_H

#include <stdint.h>

typedef enum
{
    SOUND_PROFILE_TWINKLE = 0,
    SOUND_PROFILE_TETRIS = 1
} SoundProfile;

void AudioTask_Init(void);
void StartAudioTask(void* argument);
void AudioTask_PushMask(uint8_t inputMask);
void AudioTask_SetProfile(SoundProfile profile);
void AudioTask_PlayMask(uint8_t inputMask);

#endif /* AUDIO_TASK_H */
