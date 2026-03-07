#include <gui/score_view_screen/Score_viewView.hpp>

Score_viewView::Score_viewView()
{

}

void Score_viewView::setupScreen()
{
    Score_viewViewBase::setupScreen();

    const ScoreSummary summary = presenter->getScoreSummary();

    updateScoreText(Perfect_text, Perfect_textBuffer, PERFECT_TEXT_SIZE, summary.perfect);
    updateScoreText(Great_text, Great_textBuffer, GREAT_TEXT_SIZE, summary.great);
    updateScoreText(Good_text, Good_textBuffer, GOOD_TEXT_SIZE, summary.good);
    updateScoreText(Miss_text, Miss_textBuffer, MISS_TEXT_SIZE, summary.miss);
}

void Score_viewView::tearDownScreen()
{
    Score_viewViewBase::tearDownScreen();
}

void Score_viewView::updateScoreText(touchgfx::TextAreaWithOneWildcard& textArea, touchgfx::Unicode::UnicodeChar* buffer, uint16_t bufferSize, uint16_t value)
{
    textArea.invalidate();
    Unicode::snprintf(buffer, bufferSize, "%d", value);
    textArea.resizeToCurrentText();
    textArea.invalidate();
}
