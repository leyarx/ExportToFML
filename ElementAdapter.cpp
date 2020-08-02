#include "ElementAdapter.hpp"
#include "IAPIOutputManager.hpp"
#include "IAPIOutputAdapterFactory.hpp"
#include "FMLFileOutputManager.hpp"

ElementAdapter::ElementAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory) :
	GSAPI::IAPIOutputAdapter<API_Element> (manager, factory)
{
}

ElementAdapter::ElementAdapter (const ElementAdapter& adapter) :
	GSAPI::IAPIOutputAdapter<API_Element> (adapter)
{
}

ElementAdapter::~ElementAdapter ()
{
}

ElementAdapter& ElementAdapter::operator= (const ElementAdapter& adapter)
{
	static_cast<GSAPI::IAPIOutputAdapter<API_Element>&> (*this) = adapter;
	return (*this);
}

void ElementAdapter::Output (GS::OChannel& oChannel)
{
	if ((DBERROR (outputManager == nullptr)) || (DBERROR (outputAdapterFactory == nullptr)) || (DBERROR (element == nullptr)) || (DBERROR (dbInfo == nullptr))) {
		return;
	}
    
    FMLFileOutputManager* m = dynamic_cast<FMLFileOutputManager*>(outputManager);
    
    if (m->floorInd == element->header.floorInd){
/*
    oChannel <<
            "F: " << element->header.floorInd <<
            " T: " << element->header.typeID << "\n";
*/
        switch (element->header.typeID) {
            case API_WallID:
            {
                GSAPI::IAPIOutputAdapter<API_WallType>* wallAdapter = outputAdapterFactory->GetAdapter (element->wall, *dbInfo, attributeProxy, attributeOutputType);
                *wallAdapter >> oChannel;
                delete wallAdapter;
            }
                break;
                
            case API_WindowID:
            case API_DoorID:
            {
                GSAPI::IAPIOutputAdapter<API_WindowType>* windowAdapter = outputAdapterFactory->GetAdapter (element->window, *dbInfo, attributeProxy, attributeOutputType);
                *windowAdapter >> oChannel;
                delete windowAdapter;
            }
                break;
  
            case API_TextID:
            {
                GSAPI::IAPIOutputAdapter<API_TextType>* textAdapter = outputAdapterFactory->GetAdapter (element->text, *dbInfo, attributeProxy, attributeOutputType);
                *textAdapter >> oChannel;
                delete textAdapter;
            }
                break;
 
            case API_ZoneID:
            {
                GSAPI::IAPIOutputAdapter<API_ZoneType>* zoneAdapter = outputAdapterFactory->GetAdapter (element->zone, *dbInfo, attributeProxy, attributeOutputType);
                *zoneAdapter >> oChannel;
                delete zoneAdapter;
            }
                break;
                
            case API_ObjectID:
            {
                GSAPI::IAPIOutputAdapter<API_ObjectType>* objectAdapter = outputAdapterFactory->GetAdapter (element->object, *dbInfo, attributeProxy, attributeOutputType);
                *objectAdapter >> oChannel;
                delete objectAdapter;
            }
                break;
                
            case API_DimensionID:
            {
                GSAPI::IAPIOutputAdapter<API_DimensionType>* dimensionAdapter = outputAdapterFactory->GetAdapter (element->dimension, *dbInfo, attributeProxy, attributeOutputType);
                *dimensionAdapter >> oChannel;
                delete dimensionAdapter;
            }
                break;
                
            case API_RoofID:
            {
                GSAPI::IAPIOutputAdapter<API_RoofType>* roofAdapter = outputAdapterFactory->GetAdapter (element->roof, *dbInfo, attributeProxy, attributeOutputType);
                *roofAdapter >> oChannel;
                delete roofAdapter;
            }
                break;

            case API_LineID:
            {
                GSAPI::IAPIOutputAdapter<API_LineType>* lineAdapter = outputAdapterFactory->GetAdapter (element->line, *dbInfo, attributeProxy, attributeOutputType);
                *lineAdapter >> oChannel;
                delete lineAdapter;
            }
                break;
                
            default:
                break;
        }
    }
}

void ElementAdapter::operator>> (GS::OChannel& oChannel)
{
	Output (oChannel);
}
