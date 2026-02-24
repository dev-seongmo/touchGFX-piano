#ifndef SELECT_STARVIEW_HPP
#define SELECT_STARVIEW_HPP

#include <gui_generated/select_star_screen/select_starViewBase.hpp>
#include <gui/select_star_screen/select_starPresenter.hpp>

class select_starView : public select_starViewBase
{
public:
    select_starView();
    virtual ~select_starView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SELECT_STARVIEW_HPP
