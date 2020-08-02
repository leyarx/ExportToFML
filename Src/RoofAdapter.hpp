#ifndef __ROOFADAPTER_HPP__
#define __ROOFADAPTER_HPP__

#include "IAPIOutputAdapter.hpp"

class RoofAdapter : public GSAPI::IAPIOutputAdapter<API_RoofType> {
public:
    RoofAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory);
    RoofAdapter (const RoofAdapter& adapter);
    virtual ~RoofAdapter ();
    
    RoofAdapter& operator= (const RoofAdapter& adapter);
    
    virtual void Output (GS::OChannel& oChannel) override;
    virtual void operator>> (GS::OChannel& oChannel) override;
};

#endif // __ROOFADAPTER_HPP__
