#ifndef __ZONEADAPTER_HPP__
#define __ZONEADAPTER_HPP__

#include "IAPIOutputAdapter.hpp"

class ZoneAdapter : public GSAPI::IAPIOutputAdapter<API_ZoneType> {
public:
    ZoneAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory);
    ZoneAdapter (const ZoneAdapter& adapter);
    virtual ~ZoneAdapter ();
    
    ZoneAdapter& operator= (const ZoneAdapter& adapter);
    
    virtual void Output (GS::OChannel& oChannel) override;
    virtual void operator>> (GS::OChannel& oChannel) override;
};

#endif // __ZONEADAPTER_HPP__
