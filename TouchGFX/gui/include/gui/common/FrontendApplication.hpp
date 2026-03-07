#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>
#include <touchgfx/transitions/NoTransition.hpp>

class FrontendHeap;
class Score_viewPresenter;
class Score_viewView;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleTickEvent()
    {
        model.tick();
        FrontendApplicationBase::handleTickEvent();
    }

    void gotoScore_viewScreenNoTransition();
private:
};

#endif // FRONTENDAPPLICATION_HPP
