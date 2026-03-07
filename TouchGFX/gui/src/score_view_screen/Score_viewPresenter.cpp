#include <gui/score_view_screen/Score_viewView.hpp>
#include <gui/score_view_screen/Score_viewPresenter.hpp>

Score_viewPresenter::Score_viewPresenter(Score_viewView& v)
    : view(v)
{

}

void Score_viewPresenter::activate()
{

}

void Score_viewPresenter::deactivate()
{

}

ScoreSummary Score_viewPresenter::getScoreSummary() const
{
    return model->getScoreSummary();
}
