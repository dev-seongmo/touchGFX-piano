#ifndef PLAY_VIEWVIEW_HPP
#define PLAY_VIEWVIEW_HPP

#include <stdint.h>
#include <gui_generated/play_view_screen/play_viewViewBase.hpp>
#include <gui/play_view_screen/play_viewPresenter.hpp>
#include <gui/play_view_screen/song_data.hpp>

class play_viewView : public play_viewViewBase
{
public:
    play_viewView();
    virtual ~play_viewView() {}
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

#endif // PLAY_VIEWVIEW_HPP
