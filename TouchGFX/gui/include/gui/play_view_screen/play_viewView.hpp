#ifndef PLAY_VIEWVIEW_HPP
#define PLAY_VIEWVIEW_HPP

#include <gui_generated/play_view_screen/play_viewViewBase.hpp>
#include <gui/play_view_screen/play_viewPresenter.hpp>

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
    void updateJudgeText(const char* txt);
};

#endif // PLAY_VIEWVIEW_HPP
