#ifndef SELECT_TETRISVIEW_HPP
#define SELECT_TETRISVIEW_HPP

#include <gui_generated/select_tetris_screen/select_tetrisViewBase.hpp>
#include <gui/select_tetris_screen/select_tetrisPresenter.hpp>

class select_tetrisView : public select_tetrisViewBase
{
public:
    select_tetrisView();
    virtual ~select_tetrisView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SELECT_TETRISVIEW_HPP
