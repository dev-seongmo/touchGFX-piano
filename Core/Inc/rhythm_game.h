#ifndef RHYTHM_GAME_H
#define RHYTHM_GAME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    RHYTHM_SOUND_TWINKLE = 0,
    RHYTHM_SOUND_TETRIS = 1
} RhythmSoundProfile;

typedef struct
{
    uint32_t song_ms;
    uint8_t playing;
    uint8_t input_mask;
} RhythmSharedState;

void RhythmGame_Init(void);
void RhythmGame_StartSong(void);
void RhythmGame_StopSong(void);
void RhythmGame_SetSoundProfile(RhythmSoundProfile profile);
void RhythmGame_GetState(RhythmSharedState* state);
void StartEngineTask(void* argument);

#ifdef __cplusplus
}
#endif

#endif /* RHYTHM_GAME_H */
