#include <gui/play_view_screen/play_viewView.hpp>
extern "C" volatile uint8_t highlight1_visible;
extern "C" volatile uint8_t highlight2_visible;
extern "C" volatile uint8_t highlight3_visible;
extern "C" volatile uint8_t highlight4_visible;
extern "C" volatile uint8_t highlight5_visible;
extern "C" volatile uint8_t highlight6_visible;
extern "C" volatile uint8_t highlight7_visible;
extern "C" volatile uint8_t highlight8_visible;


play_viewView::play_viewView()
{

    
}
//ai assisted code becase too slow
void play_viewView::handleTickEvent()
{
    play_viewViewBase::handleTickEvent();
    static uint8_t prev1 = 255;
    static uint8_t prev2 = 255;
    static uint8_t prev3 = 255;
    static uint8_t prev4 = 255;
    static uint8_t prev5 = 255;
    static uint8_t prev6 = 255;
    static uint8_t prev7 = 255;
    static uint8_t prev8 = 255;

    if (prev1 != highlight1_visible) {
        prev1 = highlight1_visible;
        highlight_1.setVisible(prev1);
        highlight_1.invalidate();
    }

    if (prev2 != highlight2_visible) {
        prev2 = highlight2_visible;
        highlight_2.setVisible(prev2);
        highlight_2.invalidate();
    }

    if (prev3 != highlight3_visible) {
        prev3 = highlight3_visible;
        highlight_3.setVisible(prev3);
        highlight_3.invalidate();
    }

    if (prev4 != highlight4_visible) {
        prev4 = highlight4_visible;
        highlight_4.setVisible(prev4);
        highlight_4.invalidate();
    }

    if (prev5 != highlight5_visible) {
        prev5 = highlight5_visible;
        highlight_5.setVisible(prev5);
        highlight_5.invalidate();
    }

    if (prev6 != highlight6_visible) {
        prev6 = highlight6_visible;
        highlight_6.setVisible(prev6);
        highlight_6.invalidate();
    }

    if (prev7 != highlight7_visible) {
        prev7 = highlight7_visible;
        highlight_7.setVisible(prev7);
        highlight_7.invalidate();
    }

    if (prev8 != highlight8_visible) {
        prev8 = highlight8_visible;
        highlight_8.setVisible(prev8);
        highlight_8.invalidate();
    }

}

void play_viewView::setupScreen()
{
    play_viewViewBase::setupScreen();
}

void play_viewView::tearDownScreen()
{
    play_viewViewBase::tearDownScreen();
}
