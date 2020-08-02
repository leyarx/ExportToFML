#ifndef __TEXTADAPTER_HPP__
#define __TEXTADAPTER_HPP__

#include "IAPIOutputAdapter.hpp"

class TextAdapter : public GSAPI::IAPIOutputAdapter<API_TextType> {
public:
    TextAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory);
    TextAdapter (const TextAdapter& adapter);
    virtual ~TextAdapter ();
    
    TextAdapter& operator= (const TextAdapter& adapter);
    
    virtual void Output (GS::OChannel& oChannel) override;
    virtual void operator>> (GS::OChannel& oChannel) override;
};

#endif // __TEXTADAPTER_HPP__
