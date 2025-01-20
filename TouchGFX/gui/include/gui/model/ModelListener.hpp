#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }
    virtual void setPressionEncoder(Model::Enc_Type e, Model::Enc_Pression p){};
    virtual void setOffsetEncoder(Model::Enc_Type e,S16 offset,S8 dir){};
    virtual void setWireType(viperdef_TipoFilo_e wiretype ){};
    virtual void setWireDiameter(viperdef_DiametroFilo_e wireDiameter){};
    virtual void setGasType(viperdef_TipoGas_e gastype ){};
    virtual void setMigType(viperdef_TipoMig_e  migtype){};
    virtual void setMigWeldingProcessMenu(viperui_AttualeSelezioneProcesso_e selection){};
    virtual void setProcessWelding(viperdef_Processo_e process){};

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
