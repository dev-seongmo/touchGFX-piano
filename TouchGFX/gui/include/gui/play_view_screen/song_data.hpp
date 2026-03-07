#ifndef PLAY_VIEW_SONG_DATA_HPP
#define PLAY_VIEW_SONG_DATA_HPP

#include <stdint.h>

struct NoteEvent
{
    uint8_t lane;
    uint32_t hit_ms;
};

struct SongChart
{
    const char* name;
    const NoteEvent* notes;
    uint16_t note_count;
};

const SongChart& getTwinkleTwinkleSong();
const SongChart& getTetrisThemeSong();

#endif // PLAY_VIEW_SONG_DATA_HPP
