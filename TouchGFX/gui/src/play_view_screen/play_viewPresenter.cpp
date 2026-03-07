#include <gui/play_view_screen/play_viewView.hpp>
#include <gui/play_view_screen/play_viewPresenter.hpp>

play_viewPresenter::play_viewPresenter(play_viewView& v)
    : view(v)
{

}

void play_viewPresenter::activate()
{

}

void play_viewPresenter::deactivate()
{

}

uint32_t play_viewPresenter::getSongMs() const
{
    return model->getSongMs();
}

uint8_t play_viewPresenter::getInputMask() const
{
    return model->getInputMask();
}

void play_viewPresenter::startSong()
{
    model->startSong();
}

void play_viewPresenter::stopSong()
{
    model->stopSong();
}

void play_viewPresenter::setSoundProfile(RhythmSoundProfile profile)
{
    model->setSoundProfile(profile);
}

void play_viewPresenter::resetScore()
{
    model->resetScore();
}

void play_viewPresenter::addPerfect()
{
    model->addPerfect();
}

void play_viewPresenter::addGreat()
{
    model->addGreat();
}

void play_viewPresenter::addGood()
{
    model->addGood();
}

void play_viewPresenter::addMiss()
{
    model->addMiss();
}
