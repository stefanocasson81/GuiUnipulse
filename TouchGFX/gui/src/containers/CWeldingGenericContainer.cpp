#include <gui/containers/CWeldingGenericContainer.hpp>

CWeldingGenericContainer::CWeldingGenericContainer()
{
   this->setXY(0, 40);
   this->invalidate();
}

void CWeldingGenericContainer::initialize()
{
    CWeldingGenericContainerBase::initialize();
}
void CWeldingGenericContainer::setMenuSelectedView_Callback(GenericCallback<U8,U8>& callback)
{
   MenuSelected_Callback = &callback;
}
