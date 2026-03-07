#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>
#include "rhythm_game.h"

class ModelListener;

struct ScoreSummary
{
    uint16_t perfect;
    uint16_t great;
    uint16_t good;
    uint16_t miss;
};

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    uint32_t getSongMs() const;
    uint8_t getInputMask() const;
    void startSong();
    void stopSong();
    void setSoundProfile(RhythmSoundProfile profile);
    void resetScore();
    void addPerfect();
    void addGreat();
    void addGood();
    void addMiss();
    ScoreSummary getScoreSummary() const;
protected:
    ModelListener* modelListener;
    uint32_t songMs;
    uint8_t inputMask;
    ScoreSummary scoreSummary;
};

#endif // MODEL_HPP
