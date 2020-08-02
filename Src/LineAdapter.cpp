#include "LineAdapter.hpp"
#include "IAttributeProxy.hpp"
#include "APICommon.h"


LineAdapter::LineAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory) :
GSAPI::IAPIOutputAdapter<API_LineType> (manager, factory)
{
}

LineAdapter::LineAdapter (const LineAdapter& adapter) :
GSAPI::IAPIOutputAdapter<API_LineType> (adapter)
{
}

LineAdapter::~LineAdapter ()
{
}

LineAdapter& LineAdapter::operator= (const LineAdapter& adapter)
{
    static_cast<GSAPI::IAPIOutputAdapter<API_LineType>&> (*this) = adapter;
    return (*this);
}

void LineAdapter::Output (GS::OChannel& oChannel)
{
    if ((DBERROR (outputManager == nullptr)) || (DBERROR (outputAdapterFactory == nullptr)) || (DBERROR (element == nullptr)) || (DBERROR (dbInfo == nullptr))) {
        return;
    }
    
    //const API_PenType& pen = attributeProxy->GetProxiedAttribute (*dbInfo, API_PenID, element->contPen).pen;
    
    API_ElementMemo memo;
    GSErrCode        err;

        /*
         for (int i = (*memo.pends) [1-1] + 1; i < (*memo.pends) [1]; i++) {
         oChannel <<
         (*memo.coords) [i].x << " " << (*memo.coords) [i].y << "\n";
         }
         */

    API_Attribute        attrib, attrPen;
    
    attrib.header.typeID = API_LinetypeID;
    attrib.header.index = element->ltypeInd;
    
    err = ACAPI_Attribute_Get (&attrib);
    if (err != NoError) {
        return;
    }
    
    BNZeroMemory (&attrPen, sizeof (attrPen));
    attrPen.header.typeID = API_PenID;
    attrPen.header.index  = element->linePen.penIndex;
    ACAPI_Attribute_Get (&attrPen);
    
    char    rgbhex[8];
    sprintf(rgbhex, "#%02x%02x%02x", static_cast<int> (attrPen.pen.rgb.f_red * 255.0), static_cast<int> (attrPen.pen.rgb.f_green * 255.0), static_cast<int> (attrPen.pen.rgb.f_blue * 255.0));
    
        /*
         <line>
         <type>solid_line</type>
         <points>-0.00448 4.97760 0.00000 5.03584 4.97760 0.00000</points>
         <color>0</color>
         <thickness>0.02000</thickness>
         </line>
         */
    
    
    oChannel << "<line>\n";
    // TODO make decision of line type based of dash gap period
    oChannel << "<type>";
    switch (attrib.linetype.type)
    {
        case APILine_DashedLine:
            if (attrib.linetype.nItems == 1)
            {
                oChannel << "dashed_line";
            }
            else if (attrib.linetype.nItems == 2)
            {
                oChannel << "dashdotted_line";
            }
            break;
        case APILine_SolidLine:
        default:
            oChannel << "solid_line";
            break;
    }
    oChannel << "</type>\n";
    
    oChannel << "<points>" <<
        element->begC.x << " " << element->begC.y * -1 << " 0 " <<
        element->endC.x << " " << element->endC.y * -1 << " 0" <<
        "</points>\n" <<
        "<thickness>" << element->penWeight / 10 << "</thickness>\n";
        oChannel << "<color>" << rgbhex << "</color>\n" <<
        "</line>\n";
        /*
         oChannel << "Line (guid:" << APIGuid2GSGuid (element->head.guid).ToUniString ().ToCStr () <<
         " begC:(" << element->begC.x << ", " << element->begC.y << ")" <<
         " endC:(" << element->endC.x << ", " << element->endC.y << ")" <<
         " color:(R" << static_cast<int> (pen.rgb.f_red * 255.0) <<
         ", G" << static_cast<int> (pen.rgb.f_green * 255.0) <<
         ", B" << static_cast<int> (pen.rgb.f_blue * 255.0) << "))\n";
         */
    
}

void LineAdapter::operator>> (GS::OChannel& oChannel)
{
    Output (oChannel);
}
