#ifndef SELECT_STARPRESENTER_HPP
#define SELECT_STARPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class select_starView;

class select_starPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    select_starPresenter(select_starView& v);

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

    virtual ~select_starPresenter() {}

private:
    select_starPresenter();

    select_starView& view;
};

#endif // SELECT_STARPRESENTER_HPP
