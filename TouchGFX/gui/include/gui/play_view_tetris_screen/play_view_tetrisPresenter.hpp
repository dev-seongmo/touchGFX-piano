#ifndef PLAY_VIEW_TETRISPRESENTER_HPP
#define PLAY_VIEW_TETRISPRESENTER_HPP

#include "rhythm_game.h"
#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class play_view_tetrisView;

class play_view_tetrisPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    play_view_tetrisPresenter(play_view_tetrisView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~play_view_tetrisPresenter() {}
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

private:
    play_view_tetrisPresenter();

    play_view_tetrisView& view;
};

#endif // PLAY_VIEW_TETRISPRESENTER_HPP
