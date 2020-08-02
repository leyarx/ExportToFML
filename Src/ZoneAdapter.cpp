#include "ZoneAdapter.hpp"
#include "IAttributeProxy.hpp"
#include "APICommon.h"


ZoneAdapter::ZoneAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory) :
GSAPI::IAPIOutputAdapter<API_ZoneType> (manager, factory)
{
}

ZoneAdapter::ZoneAdapter (const ZoneAdapter& adapter) :
GSAPI::IAPIOutputAdapter<API_ZoneType> (adapter)
{
}

ZoneAdapter::~ZoneAdapter ()
{
}

ZoneAdapter& ZoneAdapter::operator= (const ZoneAdapter& adapter)
{
    static_cast<GSAPI::IAPIOutputAdapter<API_ZoneType>&> (*this) = adapter;
    return (*this);
}

void ZoneAdapter::Output (GS::OChannel& oChannel)
{
    if ((DBERROR (outputManager == nullptr)) || (DBERROR (outputAdapterFactory == nullptr)) || (DBERROR (element == nullptr)) || (DBERROR (dbInfo == nullptr))) {
        return;
    }

    double level = element->roomBaseLev;
    
    API_ElementMemo memo;
    GSErrCode        err;
    
    BNZeroMemory (&memo, sizeof (API_ElementMemo));
    err = ACAPI_Element_GetMemo (element->head.guid, &memo, APIMemoMask_Polygon);
    
    if (err != NoError) {
        return;
    }
 /*
    const char * Zone = *memo.ZoneContent;
 */
    //(unsigned short*)Zone
    
   /*
    oChannel << "typeId: " << dbInfo->typeID << "\n" <<
    "Title: " << GS::UniString (dbInfo->title).ToCStr ().Get () << "\n" <<
    "name: " << GS::UniString (dbInfo->name).ToCStr ().Get () << "\n" <<
    "ref: " << GS::UniString (dbInfo->ref).ToCStr ().Get () << "\n";
    oChannel << "\n";
 */
    int         i, begInd, endInd;
    API_Coord   begC, endC;
    
    //element->poly
    //element->floorFillPen
    //const API_PenType& pen = attributeProxy->GetProxiedAttribute (*dbInfo, API_PenID, 1).pen;
 
    API_Attribute        attrib;
    

    attrib.header.typeID = API_ZoneCatID;
    attrib.header.index = element->catInd;

    err = ACAPI_Attribute_Get (&attrib);
    if (err != NoError) {
        return;
    }
    API_RGBColor& rgb = attrib.zoneCat.rgb;
    char    rgbhex[8];
    sprintf(rgbhex, "#%02x%02x%02x", static_cast<int> (rgb.f_red * 255.0), static_cast<int> (rgb.f_green * 255.0), static_cast<int> (rgb.f_blue * 255.0));
    
/*
    attrib.header.typeID = API_PenID;
    attrib.header.index = 94;
    
    err = ACAPI_Attribute_Get (&attrib);
    if (err != NoError) {
        return;
    }

    //writer->WriteBlock ("PEN", i);
    //writer->WriteRGB (&attrib.pen.rgb);
    //err = writer->WrEndblk ();
    API_PenType& pen = attrib.pen;
    
    char    rgbhex[8];
    sprintf(rgbhex, "#%02x%02x%02x", static_cast<int> (pen.rgb.f_red * 255.0), static_cast<int> (pen.rgb.f_green * 255.0), static_cast<int> (pen.rgb.f_blue * 255.0));
    
    oChannel << "pen: " << element->floorFillPen << " bg: " << element->floorFillBGPen << " l: " << element->floorContLPen << "\n";
    
    oChannel <<
    " color:(R" << static_cast<int> (pen.rgb.f_red * 255.0) <<
    ", G" << static_cast<int> (pen.rgb.f_green * 255.0) <<
    ", B" << static_cast<int> (pen.rgb.f_blue * 255.0) << "))" <<
    "width: " << pen.width << "\n";
 */
    
    
    
    oChannel << "<area>\n";
    // TODO fill
    
    if (element->useFloorFill)
    {
        API_Attribute        attrFill;
        
        attrFill.header.typeID = API_FilltypeID;
        attrFill.header.index = element->floorFillInd;
        
        err = ACAPI_Attribute_Get (&attrFill);
        if (err != NoError) {
            return;
        }
        
        if (CHSearchSubstring("rs", attrFill.filltype.head.name))
        {
            oChannel << "<asset refid=\"" << attrFill.filltype.head.name << "\"/>\n";
        }
        //oChannel << "<rotation>0 0 0</rotation>\n";
        //oChannel << "<sx>100</sx>\n";
        //oChannel << "<sy>100</sy>\n";
    }
    
    if (element->manual){
        oChannel << "<type>custom_area</type>\n";
        oChannel << "<layer>2</layer>\n";
    }
    else {
        oChannel << "<type>generated_area</type>\n";
        oChannel << "<layer>0</layer>\n";
    }
    // check zone cat name for '<' symbol, and if it exist hide zone in 3D
    if (CHSearchCharRight("<", attrib.zoneCat.head.name))
    {
        // TODO: maybe better to change to story level height
        level = 3.0;
        oChannel << "<hideIn3D>true</hideIn3D>\n";
    }
    
    oChannel << "<points>";
    
    begInd = (*memo.pends) [1-1] + 1;
    endInd = (*memo.pends) [1];
    
    for (i = begInd; i < endInd; i++) {
        begC = (*memo.coords) [i];
        endC = (*memo.coords) [i+1];
        oChannel <<
        begC.x << " " << -begC.y << " " << level << " " <<
        endC.x << " " << -endC.y << " " << level;
        if (i != endInd - 1)
        {
            oChannel << ",";
        }
    }
    
    oChannel << "</points>\n" <<
    "<color>" << rgbhex << "</color>\n";

    oChannel << "</area>\n";
    
    /*
     oChannel << "Zone (guid:" << APIGuid2GSGuid (element->head.guid).ToUniString ().ToCStr () <<
     " begC:(" << element->begC.x << ", " << element->begC.y << ")" <<
     " endC:(" << element->endC.x << ", " << element->endC.y << ")" <<
     " color:(R" << static_cast<int> (pen.rgb.f_red * 255.0) <<
     ", G" << static_cast<int> (pen.rgb.f_green * 255.0) <<
     ", B" << static_cast<int> (pen.rgb.f_blue * 255.0) << "))\n";
     */
    
}

void ZoneAdapter::operator>> (GS::OChannel& oChannel)
{
    Output (oChannel);
}
