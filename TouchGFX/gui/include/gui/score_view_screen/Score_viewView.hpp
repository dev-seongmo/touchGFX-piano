#ifndef SCORE_VIEWVIEW_HPP
#define SCORE_VIEWVIEW_HPP

#include <gui/model/Model.hpp>
#include <gui_generated/score_view_screen/Score_viewViewBase.hpp>
#include <gui/score_view_screen/Score_viewPresenter.hpp>

class Score_viewView : public Score_viewViewBase
{
public:
    Score_viewView();
    virtual ~Score_viewView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
private:
    void updateScoreText(touchgfx::TextAreaWithOneWildcard& textArea, touchgfx::Unicode::UnicodeChar* buffer, uint16_t bufferSize, uint16_t value);
};

#endif // SCORE_VIEWVIEW_HPP
