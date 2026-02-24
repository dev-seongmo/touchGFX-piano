#ifndef SELECT_TETRISPRESENTER_HPP
#define SELECT_TETRISPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class select_tetrisView;

class select_tetrisPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    select_tetrisPresenter(select_tetrisView& v);

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

    virtual ~select_tetrisPresenter() {}

private:
    select_tetrisPresenter();

    select_tetrisView& view;
};

#endif // SELECT_TETRISPRESENTER_HPP
