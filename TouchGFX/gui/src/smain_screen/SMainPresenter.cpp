#include <gui/smain_screen/SMainView.hpp>
#include <gui/smain_screen/SMainPresenter.hpp>

SMainPresenter::SMainPresenter(SMainView& v)
    : view(v)
{

}

void SMainPresenter::activate()
{
   model->setCurrentScreen(Model::View_Main);
}

void SMainPresenter::deactivate()
{

}
