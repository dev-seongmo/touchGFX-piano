#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "rhythm_game.h"

Model::Model() : modelListener(0),
                 songMs(0),
                 inputMask(0),
                 scoreSummary{0, 0, 0, 0}
{

}

void Model::tick()
{
    RhythmSharedState state;
    RhythmGame_GetState(&state);
    songMs = state.song_ms;
    inputMask = state.input_mask;
}

uint32_t Model::getSongMs() const
{
    return songMs;
}

uint8_t Model::getInputMask() const
{
    return inputMask;
}

void Model::startSong()
{
    resetScore();
    RhythmGame_StartSong();
}

void Model::stopSong()
{
    RhythmGame_StopSong();
}

void Model::setSoundProfile(RhythmSoundProfile profile)
{
    RhythmGame_SetSoundProfile(profile);
}

void Model::resetScore()
{
    scoreSummary.perfect = 0;
    scoreSummary.great = 0;
    scoreSummary.good = 0;
    scoreSummary.miss = 0;
}

void Model::addPerfect()
{
    scoreSummary.perfect++;
}

void Model::addGreat()
{
    scoreSummary.great++;
}

void Model::addGood()
{
    scoreSummary.good++;
}

void Model::addMiss()
{
    scoreSummary.miss++;
}

ScoreSummary Model::getScoreSummary() const
{
    return scoreSummary;
}
