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
void CWeldingGenericContainer::setMenuSelectedView_Callback(GenericCallback<S8,S8>& callback)
{
   menuSelected_Callback = &callback;
}
void CWeldingGenericContainer::setInitContainer_Callback(GenericCallback<void>& callback)
{
   initContainer_Callback = &callback;
}
