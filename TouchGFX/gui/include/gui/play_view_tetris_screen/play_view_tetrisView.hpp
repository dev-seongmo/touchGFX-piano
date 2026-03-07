#ifndef PLAY_VIEW_TETRISVIEW_HPP
#define PLAY_VIEW_TETRISVIEW_HPP

#include <stdint.h>
#include <gui_generated/play_view_tetris_screen/play_view_tetrisViewBase.hpp>
#include <gui/play_view_tetris_screen/play_view_tetrisPresenter.hpp>
#include <gui/play_view_screen/song_data.hpp>

class play_view_tetrisView : public play_view_tetrisViewBase
{
public:
    play_view_tetrisView();
    virtual ~play_view_tetrisView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
protected:
private:
    void checkLaneInput(uint8_t lane);
    int getLaneX(uint8_t lane) const;
    void updateComboText();
    void updateJudgeText(const char* txt);
    void setJudgeColor(const char* txt);
    touchgfx::Unicode::UnicodeChar judgeBuf[16];
};

#endif // PLAY_VIEW_TETRISVIEW_HPP
