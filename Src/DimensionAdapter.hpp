#ifndef __DIMENSIONADAPTER_HPP__
#define __DIMENSIONADAPTER_HPP__

#include "IAPIOutputAdapter.hpp"

class DimensionAdapter : public GSAPI::IAPIOutputAdapter<API_DimensionType> {
public:
    DimensionAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory);
    DimensionAdapter (const DimensionAdapter& adapter);
    virtual ~DimensionAdapter ();
    
    DimensionAdapter& operator= (const DimensionAdapter& adapter);
    
    virtual void Output (GS::OChannel& oChannel) override;
    virtual void operator>> (GS::OChannel& oChannel) override;
};

#endif // __DIMENSIONADAPTER_HPP__
