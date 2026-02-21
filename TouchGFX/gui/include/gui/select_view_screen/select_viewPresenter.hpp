#ifndef SELECT_VIEWPRESENTER_HPP
#define SELECT_VIEWPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class select_viewView;

class select_viewPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    select_viewPresenter(select_viewView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~select_viewPresenter() {}

private:
    select_viewPresenter();

    select_viewView& view;
};

#endif // SELECT_VIEWPRESENTER_HPP
