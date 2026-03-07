#include <gui/play_view_tetris_screen/play_view_tetrisView.hpp>
#include <gui/play_view_tetris_screen/play_view_tetrisPresenter.hpp>

play_view_tetrisPresenter::play_view_tetrisPresenter(play_view_tetrisView& v)
    : view(v)
{

}

void play_view_tetrisPresenter::activate()
{

}

void play_view_tetrisPresenter::deactivate()
{

}

uint32_t play_view_tetrisPresenter::getSongMs() const
{
    return model->getSongMs();
}

uint8_t play_view_tetrisPresenter::getInputMask() const
{
    return model->getInputMask();
}

void play_view_tetrisPresenter::startSong()
{
    model->startSong();
}

void play_view_tetrisPresenter::stopSong()
{
    model->stopSong();
}

void play_view_tetrisPresenter::setSoundProfile(RhythmSoundProfile profile)
{
    model->setSoundProfile(profile);
}

void play_view_tetrisPresenter::resetScore()
{
    model->resetScore();
}

void play_view_tetrisPresenter::addPerfect()
{
    model->addPerfect();
}

void play_view_tetrisPresenter::addGreat()
{
    model->addGreat();
}

void play_view_tetrisPresenter::addGood()
{
    model->addGood();
}

void play_view_tetrisPresenter::addMiss()
{
    model->addMiss();
}
