#include "DimensionAdapter.hpp"
#include "IAttributeProxy.hpp"
#include "APICommon.h"
#include "ACAPinc.h" // for ACAPI_Element_Get3DInfo


DimensionAdapter::DimensionAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory) :
GSAPI::IAPIOutputAdapter<API_DimensionType> (manager, factory)
{
}

DimensionAdapter::DimensionAdapter (const DimensionAdapter& adapter) :
GSAPI::IAPIOutputAdapter<API_DimensionType> (adapter)
{
}

DimensionAdapter::~DimensionAdapter ()
{
}

DimensionAdapter& DimensionAdapter::operator= (const DimensionAdapter& adapter)
{
    static_cast<GSAPI::IAPIOutputAdapter<API_DimensionType>&> (*this) = adapter;
    return (*this);
}

void DimensionAdapter::Output (GS::OChannel& oChannel)
{
    if ((DBERROR (outputManager == nullptr)) || (DBERROR (outputAdapterFactory == nullptr)) || (DBERROR (element == nullptr)) || (DBERROR (dbInfo == nullptr))) {
        return;
        }
/*
    oChannel << "Dimension!!!\n" << "nDimElem: " << element->nDimElem << "refC.x: " << element->refC.x << "refC.y: " << element->refC.y << "\n";
 */
    API_ElementMemo memo;
    GSErrCode        err;
    
    if (element->head.hasMemo) {
        err = ACAPI_Element_GetMemo (element->head.guid, &memo);
        for (int i = 0; i < element->nDimElem - 1; i++)
        {
            oChannel <<
            "<line>\n" <<
            "<type>custom_dimension</type>\n"
            "<points>";
            // loop over dimension points and get coordinates
            for (int k = i; k < i + 2; k++) {
                oChannel << (*memo.dimElems)[k].pos.x << " " << (*memo.dimElems)[k].pos.y * -1 << " 0 ";
            }
            oChannel <<
            "</points>\n" <<
            "<color>#000000</color>\n" <<
            "<thickness>0.010</thickness>\n" <<
            "</line>\n";
        }
    }
}

void DimensionAdapter::operator>> (GS::OChannel& oChannel)
{
    Output (oChannel);
}
