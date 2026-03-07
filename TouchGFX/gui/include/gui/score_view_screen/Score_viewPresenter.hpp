#ifndef SCORE_VIEWPRESENTER_HPP
#define SCORE_VIEWPRESENTER_HPP

#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Score_viewView;

class Score_viewPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Score_viewPresenter(Score_viewView& v);

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

    virtual ~Score_viewPresenter() {}
    ScoreSummary getScoreSummary() const;

private:
    Score_viewPresenter();

    Score_viewView& view;
};

#endif // SCORE_VIEWPRESENTER_HPP
