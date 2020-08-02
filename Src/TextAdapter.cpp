#include "TextAdapter.hpp"
#include "IAttributeProxy.hpp"
#include "APICommon.h"
#include "CH.hpp"


TextAdapter::TextAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory) :
GSAPI::IAPIOutputAdapter<API_TextType> (manager, factory)
{
}

TextAdapter::TextAdapter (const TextAdapter& adapter) :
GSAPI::IAPIOutputAdapter<API_TextType> (adapter)
{
}

TextAdapter::~TextAdapter ()
{
}

TextAdapter& TextAdapter::operator= (const TextAdapter& adapter)
{
    static_cast<GSAPI::IAPIOutputAdapter<API_TextType>&> (*this) = adapter;
    return (*this);
}

void TextAdapter::Output (GS::OChannel& oChannel)
{
    if ((DBERROR (outputManager == nullptr)) || (DBERROR (outputAdapterFactory == nullptr)) || (DBERROR (element == nullptr)) || (DBERROR (dbInfo == nullptr))) {
        return;
    }
 
    API_ElementMemo memo;
    GSErrCode        err;
    
    BNZeroMemory (&memo, sizeof (API_ElementMemo));
    err = ACAPI_Element_GetMemo (element->head.guid, &memo, APIMemoMask_TextContent);
  
    if (err != NoError) {
        return;
    }
    
    // get the center of the text block
    // TODO: check for text rotation!!!!
    API_Coord anchor;
    
    switch (element->anchor) {
        case APIAnc_LT:
            anchor.x = element->width / 2;
            anchor.y = -element->height / 2;
            break;
            
        case APIAnc_MT:
            anchor.x = 0;
            anchor.y = -element->height / 2;
            break;
            
        case APIAnc_RT:
            anchor.x = -element->width / 2;
            anchor.y = -element->height / 2;
            break;
            
        case APIAnc_LM:
            anchor.x = element->width / 2;
            anchor.y = 0;
            break;
            
        case APIAnc_MM:
            anchor.x = 0;
            anchor.y = 0;
            break;
            
        case APIAnc_RM:
            anchor.x = -element->width / 2;
            anchor.y = 0;
            break;
            
        case APIAnc_LB:
            anchor.x = element->width / 2;
            anchor.y = element->height / 2;
            break;
            
        case APIAnc_MB:
            anchor.x = 0;
            anchor.y = element->height / 2;
            break;
            
        case APIAnc_RB:
            anchor.x = -element->width / 2;
            anchor.y = element->height / 2;
            break;
    }
    
    // convert milimeters to meters
    anchor.x = anchor.x / 20;
    anchor.y = anchor.y / 20;
    
    //oChannel << "element->width: " << element->width << "element->height: " << element->height << "\n";

    //const char * text = *memo.textContent;
    //char * text = strdup(*memo.textContent);

    GS::UniString text (*memo.textContent);
    text.ReplaceAll("<", "&lt;");
    
    //GS::UniString (*memo.textContent).ReplaceAll("<", "lol");
    
    //char    textX[1024];

    //const char *buffer;
    //char *str;
   // GS::Int32 maxLen;
    //CHGetXMLString(textX, str, maxLen);
    
    //GS::UniString (*memo.textContent).ReplaceAll("<", "lol");
    
    //CHGetXMLString(textX, text, sizeof(textX));
    
    API_Attribute    attr, attrPen;
    
    BNZeroMemory (&attr, sizeof (attr));
    attr.header.typeID = API_FontID;
    attr.header.index  = element->font;
    ACAPI_Attribute_Get (&attr);
 
    BNZeroMemory (&attrPen, sizeof (attrPen));
    attrPen.header.typeID = API_PenID;
    attrPen.header.index  = element->pen;
    ACAPI_Attribute_Get (&attrPen);
    
    char    rgbhex[8];
    sprintf(rgbhex, "#%02x%02x%02x", static_cast<int> (attrPen.pen.rgb.f_red * 255.0), static_cast<int> (attrPen.pen.rgb.f_green * 255.0), static_cast<int> (attrPen.pen.rgb.f_blue * 255.0));
    
    //(unsigned short*)text
    //const API_PenType& pen = attributeProxy->GetProxiedAttribute (*dbInfo, API_PenID, element->contPen).pen;
    oChannel << "<object>\n" <<
    "<type>text</type>\n" <<
    "<points>" << element->loc.x + anchor.x << " " << (element->loc.y + anchor.y) * -1 << " 0</points>\n" <<
    "<text>" << text.ToCStr ().Get () << "</text>\n" <<
    "<align>";
    switch (element->just) {
        case APIJust_Left:
            oChannel << "left";
            break;
            
        case APIJust_Center:
        case APIJust_Full:
            oChannel << "center";
            break;
            
        case APIJust_Right:
            oChannel << "right";
            break;
    }
    oChannel <<
    "</align>\n" <<
    "<font-family>" << attr.header.name << "</font-family>\n" <<
    "<font-size>" << round(element->size * 3.4) << "</font-size>\n";

    if ((element->faceBits & APIFace_Bold) != 0)            oChannel << "<font-bold/>\n";
    if ((element->faceBits & APIFace_Italic) != 0)        oChannel << "<font-italic/>\n";
    if ((element->faceBits & APIFace_Underline) != 0)    oChannel << "<font-outline/>\n";
    
    oChannel << "<rotation>" << element->angle * RADDEG * -1 << "</rotation>\n";

    oChannel <<
    "<font-color>" << rgbhex << "</font-color>\n" <<
    "<background-color>#ffffff</background-color>\n" <<
    "<background-alpha>0</background-alpha>\n" <<
    "</object>\n";
    /*
     oChannel << "Text (guid:" << APIGuid2GSGuid (element->head.guid).ToUniString ().ToCStr () <<
     " begC:(" << element->begC.x << ", " << element->begC.y << ")" <<
     " endC:(" << element->endC.x << ", " << element->endC.y << ")" <<
     " color:(R" << static_cast<int> (pen.rgb.f_red * 255.0) <<
     ", G" << static_cast<int> (pen.rgb.f_green * 255.0) <<
     ", B" << static_cast<int> (pen.rgb.f_blue * 255.0) << "))\n";
     */
    
}

void TextAdapter::operator>> (GS::OChannel& oChannel)
{
    Output (oChannel);
}
