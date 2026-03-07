#include <gui/common/FrontendApplication.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <gui/score_view_screen/Score_viewPresenter.hpp>
#include <gui/score_view_screen/Score_viewView.hpp>
#include <touchgfx/transitions/NoTransition.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{

}

void FrontendApplication::gotoScore_viewScreenNoTransition()
{
    touchgfx::makeTransition<Score_viewView, Score_viewPresenter, touchgfx::NoTransition, Model>(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
