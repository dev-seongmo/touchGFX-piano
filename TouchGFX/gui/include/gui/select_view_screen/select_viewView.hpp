#ifndef SELECT_VIEWVIEW_HPP
#define SELECT_VIEWVIEW_HPP

#include <gui_generated/select_view_screen/select_viewViewBase.hpp>
#include <gui/select_view_screen/select_viewPresenter.hpp>

class select_viewView : public select_viewViewBase
{
public:
    select_viewView();
    virtual ~select_viewView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SELECT_VIEWVIEW_HPP
