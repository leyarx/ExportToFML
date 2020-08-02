#ifndef FMLOutputAdapterFactory_hpp
#define FMLOutputAdapterFactory_hpp

#include "NullTypeOutputAdapterFactory.hpp"


class FMLOutputAdapterFactory : public GSAPI::NullTypeOutputAdapterFactory
{
public:
    FMLOutputAdapterFactory (GSAPI::IAPIOutputManager& manager);
    FMLOutputAdapterFactory (const FMLOutputAdapterFactory& factory);
    virtual ~FMLOutputAdapterFactory ();
    
    FMLOutputAdapterFactory& operator= (const FMLOutputAdapterFactory& factory);
    
    virtual GSAPI::IAPIOutputAdapter<API_Element>*        GetAdapter (const API_Element&                forType,
                                                                      const API_DatabaseInfo&            dbInfo,
                                                                      GSAPI::IAttributeProxy*            attributeProxy        = nullptr,
                                                                      GSAPI::OutputUsedAttributesType    attributeOutputType = GSAPI::DoNotOutput,
                                                                      const API_ElementMemo*            withMemo            = nullptr) override;

    virtual GSAPI::IAPIOutputAdapter<API_WallType>*        GetAdapter (const API_WallType&                forType,
                                                                       const API_DatabaseInfo&            dbInfo,
                                                                       GSAPI::IAttributeProxy*            attributeProxy        = nullptr,
                                                                       GSAPI::OutputUsedAttributesType    attributeOutputType = GSAPI::DoNotOutput) override;

    virtual GSAPI::IAPIOutputAdapter<API_WindowType>*    GetAdapter (const API_WindowType&                forType,
                                                                     const API_DatabaseInfo&              dbInfo,
                                                                     GSAPI::IAttributeProxy*              attributeProxy        = nullptr,
                                                                     GSAPI::OutputUsedAttributesType      attributeOutputType = GSAPI::DoNotOutput) override;
    
    virtual GSAPI::IAPIOutputAdapter<API_TextType>*    GetAdapter (const API_TextType&                forType,
                                                                     const API_DatabaseInfo&              dbInfo,
                                                                     GSAPI::IAttributeProxy*              attributeProxy        = nullptr,
                                                                     GSAPI::OutputUsedAttributesType      attributeOutputType = GSAPI::DoNotOutput) override;
    
    virtual GSAPI::IAPIOutputAdapter<API_ZoneType>*    GetAdapter (const API_ZoneType&                forType,
                                                                   const API_DatabaseInfo&              dbInfo,
                                                                   GSAPI::IAttributeProxy*              attributeProxy        = nullptr,
                                                                   GSAPI::OutputUsedAttributesType      attributeOutputType = GSAPI::DoNotOutput) override;
    
    virtual GSAPI::IAPIOutputAdapter<API_ObjectType>*    GetAdapter (const API_ObjectType&                forType,
                                                                   const API_DatabaseInfo&              dbInfo,
                                                                   GSAPI::IAttributeProxy*              attributeProxy        = nullptr,
                                                                   GSAPI::OutputUsedAttributesType      attributeOutputType = GSAPI::DoNotOutput) override;
    
    virtual GSAPI::IAPIOutputAdapter<API_DimensionType>*    GetAdapter (const API_DimensionType&                forType,
                                                                     const API_DatabaseInfo&              dbInfo,
                                                                     GSAPI::IAttributeProxy*              attributeProxy        = nullptr,
                                                                     GSAPI::OutputUsedAttributesType      attributeOutputType = GSAPI::DoNotOutput) override;
    
    virtual GSAPI::IAPIOutputAdapter<API_RoofType>*    GetAdapter (const API_RoofType&                forType,
                                                                        const API_DatabaseInfo&              dbInfo,
                                                                        GSAPI::IAttributeProxy*              attributeProxy        = nullptr,
                                                                        GSAPI::OutputUsedAttributesType      attributeOutputType = GSAPI::DoNotOutput) override;
    
    virtual GSAPI::IAPIOutputAdapter<API_LineType>*    GetAdapter (const API_LineType&                forType,
                                                                   const API_DatabaseInfo&              dbInfo,
                                                                   GSAPI::IAttributeProxy*              attributeProxy        = nullptr,
                                                                   GSAPI::OutputUsedAttributesType      attributeOutputType = GSAPI::DoNotOutput) override;
};

#endif /* FMLOutputAdapterFactory_hpp */
