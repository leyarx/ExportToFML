#ifndef __LINEADAPTER_HPP__
#define __LINEADAPTER_HPP__

#include "IAPIOutputAdapter.hpp"

class LineAdapter : public GSAPI::IAPIOutputAdapter<API_LineType> {
public:
    LineAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory);
    LineAdapter (const LineAdapter& adapter);
    virtual ~LineAdapter ();
    
    LineAdapter& operator= (const LineAdapter& adapter);
    
    virtual void Output (GS::OChannel& oChannel) override;
    virtual void operator>> (GS::OChannel& oChannel) override;
};

#endif // __LINEADAPTER_HPP__
