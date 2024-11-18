#ifndef CIMPOSTAZIONERAPIDASINERGIA_HPP
#define CIMPOSTAZIONERAPIDASINERGIA_HPP

#include <gui_generated/containers/CImpostazioneRapidaSinergiaBase.hpp>

extern "C"
{
	#include "CORELib/Viper/Common/ViperDef.h"
};

class CImpostazioneRapidaSinergia : public CImpostazioneRapidaSinergiaBase
{
public:
    CImpostazioneRapidaSinergia();
    virtual ~CImpostazioneRapidaSinergia() {}

    virtual void initialize();

    void ManagerTickEvent(void);
protected:
    viperdef_TipoFilo_e TipoFiloShow;
    viperdef_DiametroFilo_e DiametroFiloShow;
    viperdef_TipoGas_e TipoGasShow;
};

#endif // CIMPOSTAZIONERAPIDASINERGIA_HPP
