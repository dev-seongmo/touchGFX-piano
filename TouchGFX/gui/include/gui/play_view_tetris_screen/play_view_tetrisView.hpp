#ifndef PLAY_VIEW_TETRISVIEW_HPP
#define PLAY_VIEW_TETRISVIEW_HPP

#include <gui_generated/play_view_tetris_screen/play_view_tetrisViewBase.hpp>
#include <gui/play_view_tetris_screen/play_view_tetrisPresenter.hpp>

class play_view_tetrisView : public play_view_tetrisViewBase
{
public:
    play_view_tetrisView();
    virtual ~play_view_tetrisView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // PLAY_VIEW_TETRISVIEW_HPP
