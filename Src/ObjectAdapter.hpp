#ifndef __OBJECTADAPTER_HPP__
#define __OBJECTADAPTER_HPP__

#include "IAPIOutputAdapter.hpp"

class ObjectAdapter : public GSAPI::IAPIOutputAdapter<API_ObjectType> {
public:
    ObjectAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory);
    ObjectAdapter (const ObjectAdapter& adapter);
    virtual ~ObjectAdapter ();
    
    ObjectAdapter& operator= (const ObjectAdapter& adapter);
    
    virtual void Output (GS::OChannel& oChannel) override;
    virtual void operator>> (GS::OChannel& oChannel) override;
};

#endif // __OBJECTADAPTER_HPP__
