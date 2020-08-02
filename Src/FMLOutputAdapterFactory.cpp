#include "FMLOutputAdapterFactory.hpp"
#include "ElementAdapter.hpp"
#include "WallAdapter.hpp"
#include "WindowDoorAdapter.hpp"
#include "TextAdapter.hpp"
#include "ZoneAdapter.hpp"
#include "ObjectAdapter.hpp"
#include "DimensionAdapter.hpp"
#include "RoofAdapter.hpp"
#include "LineAdapter.hpp"


FMLOutputAdapterFactory::FMLOutputAdapterFactory (GSAPI::IAPIOutputManager& manager) :
NullTypeOutputAdapterFactory (manager)
{
}

FMLOutputAdapterFactory::FMLOutputAdapterFactory (const FMLOutputAdapterFactory& factory) :
NullTypeOutputAdapterFactory (factory)
{
}

FMLOutputAdapterFactory::~FMLOutputAdapterFactory ()
{
}

FMLOutputAdapterFactory& FMLOutputAdapterFactory::operator= (const FMLOutputAdapterFactory& factory)
{
    NullTypeOutputAdapterFactory::operator= (factory); return (*this);
}


GSAPI::IAPIOutputAdapter<API_Element>* FMLOutputAdapterFactory::GetAdapter (const API_Element&                forType,
                                                                            const API_DatabaseInfo&            dbInfo,
                                                                            GSAPI::IAttributeProxy*            attributeProxy        /*= nullptr*/,
                                                                            GSAPI::OutputUsedAttributesType    attributeOutputType /*= GSAPI::DoNotOutput*/,
                                                                            const API_ElementMemo*            /*withMemo            = nullptr*/)
{
    ElementAdapter* adapter = new ElementAdapter (&outputManager, this);
    adapter->SetParameters (&forType, &dbInfo, attributeProxy, attributeOutputType);
    return adapter;
}

GSAPI::IAPIOutputAdapter<API_WallType>* FMLOutputAdapterFactory::GetAdapter (const API_WallType&                forType,
                                                                             const API_DatabaseInfo&            dbInfo,
                                                                             GSAPI::IAttributeProxy*            attributeProxy        /*= nullptr*/,
                                                                             GSAPI::OutputUsedAttributesType    attributeOutputType /*= GSAPI::DoNotOutput*/)
{
    WallAdapter* adapter = new WallAdapter (&outputManager, this);
    adapter->SetParameters (&forType, &dbInfo, attributeProxy, attributeOutputType);
    return adapter;
}

GSAPI::IAPIOutputAdapter<API_WindowType>* FMLOutputAdapterFactory::GetAdapter (const API_WindowType&            forType,
                                                                               const API_DatabaseInfo&            dbInfo,
                                                                               GSAPI::IAttributeProxy*            attributeProxy        /*= nullptr*/,
                                                                               GSAPI::OutputUsedAttributesType    attributeOutputType /*= GSAPI::DoNotOutput*/)
{
    WindowDoorAdapter* adapter = new WindowDoorAdapter (&outputManager, this);
    adapter->SetParameters (&forType, &dbInfo, attributeProxy, attributeOutputType);
    return adapter;
}


GSAPI::IAPIOutputAdapter<API_TextType>* FMLOutputAdapterFactory::GetAdapter (const API_TextType&            forType,
                                                                               const API_DatabaseInfo&            dbInfo,
                                                                               GSAPI::IAttributeProxy*            attributeProxy        /*= nullptr*/,
                                                                               GSAPI::OutputUsedAttributesType    attributeOutputType /*= GSAPI::DoNotOutput*/)
{
    TextAdapter* adapter = new TextAdapter (&outputManager, this);
    adapter->SetParameters (&forType, &dbInfo, attributeProxy, attributeOutputType);
    return adapter;
}


GSAPI::IAPIOutputAdapter<API_ZoneType>* FMLOutputAdapterFactory::GetAdapter (const API_ZoneType&            forType,
                                                                             const API_DatabaseInfo&            dbInfo,
                                                                             GSAPI::IAttributeProxy*            attributeProxy        /*= nullptr*/,
                                                                             GSAPI::OutputUsedAttributesType    attributeOutputType /*= GSAPI::DoNotOutput*/)
{
    ZoneAdapter* adapter = new ZoneAdapter (&outputManager, this);
    adapter->SetParameters (&forType, &dbInfo, attributeProxy, attributeOutputType);
    return adapter;
}


GSAPI::IAPIOutputAdapter<API_ObjectType>* FMLOutputAdapterFactory::GetAdapter (const API_ObjectType&            forType,
                                                                             const API_DatabaseInfo&            dbInfo,
                                                                             GSAPI::IAttributeProxy*            attributeProxy        /*= nullptr*/,
                                                                             GSAPI::OutputUsedAttributesType    attributeOutputType /*= GSAPI::DoNotOutput*/)
{
    ObjectAdapter* adapter = new ObjectAdapter (&outputManager, this);
    adapter->SetParameters (&forType, &dbInfo, attributeProxy, attributeOutputType);
    return adapter;
}


GSAPI::IAPIOutputAdapter<API_DimensionType>* FMLOutputAdapterFactory::GetAdapter (const API_DimensionType&            forType,
                                                                               const API_DatabaseInfo&            dbInfo,
                                                                               GSAPI::IAttributeProxy*            attributeProxy        /*= nullptr*/,
                                                                               GSAPI::OutputUsedAttributesType    attributeOutputType /*= GSAPI::DoNotOutput*/)
{
    DimensionAdapter* adapter = new DimensionAdapter (&outputManager, this);
    adapter->SetParameters (&forType, &dbInfo, attributeProxy, attributeOutputType);
    return adapter;
}


GSAPI::IAPIOutputAdapter<API_RoofType>* FMLOutputAdapterFactory::GetAdapter (const API_RoofType&            forType,
                                                                                  const API_DatabaseInfo&            dbInfo,
                                                                                  GSAPI::IAttributeProxy*            attributeProxy        /*= nullptr*/,
                                                                                  GSAPI::OutputUsedAttributesType    attributeOutputType /*= GSAPI::DoNotOutput*/)
{
    RoofAdapter* adapter = new RoofAdapter (&outputManager, this);
    adapter->SetParameters (&forType, &dbInfo, attributeProxy, attributeOutputType);
    return adapter;
}


GSAPI::IAPIOutputAdapter<API_LineType>* FMLOutputAdapterFactory::GetAdapter (const API_LineType&            forType,
                                                                             const API_DatabaseInfo&            dbInfo,
                                                                             GSAPI::IAttributeProxy*            attributeProxy        /*= nullptr*/,
                                                                             GSAPI::OutputUsedAttributesType    attributeOutputType /*= GSAPI::DoNotOutput*/)
{
    LineAdapter* adapter = new LineAdapter (&outputManager, this);
    adapter->SetParameters (&forType, &dbInfo, attributeProxy, attributeOutputType);
    return adapter;
}
