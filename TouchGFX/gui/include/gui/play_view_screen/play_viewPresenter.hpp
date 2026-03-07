#ifndef PLAY_VIEWPRESENTER_HPP
#define PLAY_VIEWPRESENTER_HPP

#include <stdint.h>
#include "rhythm_game.h"
#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class play_viewView;

class play_viewPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    play_viewPresenter(play_viewView& v);

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

    virtual ~play_viewPresenter() {}
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
    play_viewPresenter();

    play_viewView& view;
};

#endif // PLAY_VIEWPRESENTER_HPP
