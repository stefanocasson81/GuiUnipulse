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

    virtual void wireChanged(uint8_t type){};
    virtual void processChanged(uint8_t type){};
    virtual void setPressionEncoder(Model::Enc_Type e, Model::Enc_Pression p){};
    virtual void setOffsetEncoder(Model::Enc_Type e,S16 offset,S8 dir){};

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
