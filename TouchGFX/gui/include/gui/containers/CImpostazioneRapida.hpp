#ifndef CIMPOSTAZIONERAPIDA_HPP
#define CIMPOSTAZIONERAPIDA_HPP

#include <gui_generated/containers/CImpostazioneRapidaBase.hpp>

extern "C"
{
	#include "ARMLib/Common/Data.h"
};



class CImpostazioneRapida : public CImpostazioneRapidaBase
{
public:
    CImpostazioneRapida();
    virtual ~CImpostazioneRapida() {}

    virtual void initialize();

    void ManagerTickEvent(void);
    void SetInfo(data_Data_t *ptrData,char *ptrStrTitolo);
    void SetInfo(data_Data_t *ptrData,uint32_t enumTypeText);
    void SetIco(uint32_t icoId);
    void Seleziona(bool seleziona);

    bool Selezionato;
protected:
    data_Data_t *PtrData;
    int32_t ValShow;

};

#endif // CIMPOSTAZIONERAPIDA_HPP
