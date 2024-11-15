#ifndef SSPLASHSCREENVIEW_HPP
#define SSPLASHSCREENVIEW_HPP

#include <gui_generated/ssplashscreen_screen/SSplashScreenViewBase.hpp>
#include <gui/ssplashscreen_screen/SSplashScreenPresenter.hpp>

class SSplashScreenView : public SSplashScreenViewBase
{
public:
    SSplashScreenView();
    virtual ~SSplashScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    uint32_t OldCodiceFirmwareEsteso;
};

#endif // SSPLASHSCREENVIEW_HPP
