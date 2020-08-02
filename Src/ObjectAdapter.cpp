#include "ObjectAdapter.hpp"
#include "IAttributeProxy.hpp"
#include "APICommon.h"
#include "ACAPinc.h" // for ACAPI_Element_Get3DInfo


ObjectAdapter::ObjectAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory) :
GSAPI::IAPIOutputAdapter<API_ObjectType> (manager, factory)
{
}

ObjectAdapter::ObjectAdapter (const ObjectAdapter& adapter) :
GSAPI::IAPIOutputAdapter<API_ObjectType> (adapter)
{
}

ObjectAdapter::~ObjectAdapter ()
{
}

ObjectAdapter& ObjectAdapter::operator= (const ObjectAdapter& adapter)
{
    static_cast<GSAPI::IAPIOutputAdapter<API_ObjectType>&> (*this) = adapter;
    return (*this);
}

void ObjectAdapter::Output (GS::OChannel& oChannel)
{
    if ((DBERROR (outputManager == nullptr)) || (DBERROR (outputAdapterFactory == nullptr)) || (DBERROR (element == nullptr)) || (DBERROR (dbInfo == nullptr))) {
        return;
    }
    
    API_ElementMemo memo;
    GSErrCode        err;
    
    BNZeroMemory (&memo, sizeof (API_ElementMemo));
    err = ACAPI_Element_GetMemo (element->head.guid, &memo, APIMemoMask_Polygon);
    
    if (err != NoError) {
        return;
    }
    /*
     const char * Object = *memo.ObjectContent;
     */
    //(unsigned short*)Object
    
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
    
    
    /*
    API_Attribute        attrib;
    
    
    attrib.header.typeID = API_ObjectCatID;
    attrib.header.index = element->catInd;
    
    err = ACAPI_Attribute_Get (&attrib);
    if (err != NoError) {
        return;
    }
    API_RGBColor& rgb = attrib.zoneCat.rgb;
    char    rgbhex[8];
    sprintf(rgbhex, "#%02x%02x%02x", static_cast<int> (rgb.f_red * 255.0), static_cast<int> (rgb.f_green * 255.0), static_cast<int> (rgb.f_blue * 255.0));
  */
    
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

    API_LibPart           libPart;

    BNZeroMemory (&libPart, sizeof (API_LibPart));
    libPart.index = element->libInd;
    
    err = ACAPI_LibPart_Get (&libPart);
    if (err != NoError) {
        return;
    }

 
    
    double               a, b;
    Int32                addParNum, ind, i1, i2;
    API_AddParType       **addPars = nullptr;
    double               value;
    char                 *valueStr;
    
    GS::uchar_t         refid[API_UAddParStrLen];
    double height = 0;
    
    err = ACAPI_LibPart_GetParams (element->libInd, &a, &b, &addParNum, &addPars);
    if (err) {
        //GiveMessage_Err ("ACAPI_LibPart_GetParams", err);
        return;
    }
    for (i = 0; i < addParNum; i++) {
        // get floorplanner refid from object
        if (CHEqualCStrings((*addPars)[i].name, "FP_id"))
        {
            BNCopyMemory(refid, (*addPars)[i].value.uStr, API_UAddParStrLen);
        }
        /*
        // get height of the object
        else if (CHEqualCStringsInsensitive((*addPars)[i].name, "zzyzx"))
        {
            zzyzx = (*addPars)[i].value.real;
        }
         */
    }
    
 /*
    oChannel << "a: " << a << " b: " << b << "\n";
    for (i = 0; i < addParNum; i++) {
        if ((*addPars)[i].typeMod == API_ParSimple)
         {
            oChannel <<
                    "typeID: "      << (*addPars)[i].typeID <<
                    " name: "       << (*addPars)[i].name <<
                    " descname: "   << GS::UniString ((*addPars)[i].uDescname).ToCStr() <<
                    " flags:"       << (*addPars)[i].flags <<
                    " value.real:"  << (*addPars)[i].value.real <<
                    " value.str:"   << GS::UniString ((*addPars)[i].value.uStr).ToCStr() <<
                    "\n";
            
//            DumpOneParam ((*addPars)[i].typeID, (*addPars)[i].name, (*addPars)[i].flags,
//                          (*addPars)[i].value.real, (*addPars)[i].value.str,
//                          0,0);
            
        }
        
        else {
            ind = 0;
            oChannel <<
                "typeID: "      << (*addPars)[i].typeID <<
                " name: "       << (*addPars)[i].name <<
                " descname: "   << GS::UniString ((*addPars)[i].uDescname).ToCStr() <<
                " flags:"       << (*addPars)[i].flags <<
                " dim1:" << -(*addPars)[i].dim1 <<
                " dim2:" << -(*addPars)[i].dim2 <<
                "\n";
            
       //     DumpOneParam ((*addPars)[i].typeID, (*addPars)[i].name, (*addPars)[i].flags,
       //                   0.0, nullptr,
       //                   -(*addPars)[i].dim1, -(*addPars)[i].dim2);
            for (i1 = 1; i1 <= (*addPars)[i].dim1; i1++) {
                for (i2 = 1; i2 <= (*addPars)[i].dim2; i2++) {
                    if ((*addPars)[i].typeID != APIParT_CString) {
                        value = (Int32) ((double *) *(*addPars)[i].value.array) [ind];
                        valueStr = nullptr;
                        ind ++;
                    } else {
                        value = 0.0;
                        valueStr = *(*addPars)[i].value.array + ind;
                        ind += strlen (valueStr) + 1;
                    }
                    
                    oChannel <<
                        "typeID: "      << (*addPars)[i].typeID <<
                        " name: "       << (*addPars)[i].name <<
                        " descname: "   << GS::UniString ((*addPars)[i].uDescname).ToCStr() <<
                        " value: "      << value <<
                        " valueStr: "   << GS::UniString (valueStr).ToCStr() <<
                        " i1: "         << i1 <<
                        " i2: "         << i2 <<
                        "\n";
      //              DumpOneParam ((*addPars)[i].typeID, nullptr, 0,
      //                            value, valueStr,
      //                            i1, i2);
                }
            }
        }
    }
  */
    ACAPI_DisposeAddParHdl (&addPars);
 
    API_ElemInfo3D          info3D;
    
        err = ACAPI_Element_Get3DInfo(element->head, &info3D);
        if (err == NoError) {
            // get height of the object
            height = info3D.bounds.zMax - info3D.bounds.zMin;
      /*
            oChannel <<
            " xMax: " << info3D.bounds.xMax <<
            " xMin: " << info3D.bounds.xMin <<
            " yMax: " << info3D.bounds.yMax <<
            " yMin: " << info3D.bounds.yMin <<
            " zMax: " << info3D.bounds.zMax <<
            " zMin: " << info3D.bounds.zMin << "\n";
       */
       }

    
//    oChannel <<
//    "docu_UName: " << GS::UniString (libPart.docu_UName).ToCStr ().Get () << "\n" <<
//    "file_UName: " << GS::UniString (libPart.file_UName).ToCStr ().Get () << "\n";
    
///    GS::UniString docu_UName = GS::UniString (libPart.docu_UName);
///    GS::UniString refid = docu_UName.GetSubstring(0, docu_UName.FindFirst(GS::UniChar(' ')));
///    GS::UniString rgbhex = docu_UName.GetSubstring(GS::UniChar(' '), GS::UniChar(' '));
    // oChannel << "<asset refid=\"" << refid.ToCStr ().Get () << "\"/>\n";
    
    // check if refid is not empty
    if (refid[0] != '\0') {
        // TODO: extract zone label
        double aRad = element->angle * -1;
        double aDeg = aRad * RADDEG;
        
        oChannel << "<object>\n";

        switch (libPart.typeID) {
            case APILib_ObjectID:
            {
                oChannel << "<asset refid=\"" << GS::UniString (refid).ToCStr() << "\"/>\n";
                oChannel << "<type>furniture</type>\n";
                oChannel << "<layer>" << int(element->head.drwIndex) << "</layer>\n";
                oChannel << "<points>" <<
                element->pos.x + ((element->reflected ? -1 : 1) * (element->xRatio / 2) * cos(aRad) - (-element->yRatio / 2) * sin(aRad)) << " " <<
                    element->pos.y * -1  + ((element->reflected ? -1 : 1) * (element->xRatio / 2) * sin(aRad) + (-element->yRatio / 2) * cos(aRad)) << " " <<
                    element->level << "</points>\n" <<
                "<size>" << element->xRatio << " " << element->yRatio << " " << height << "</size>\n" <<
                "<rotation>0 0 " << aDeg << "</rotation>\n" <<
                "<mirrored>" << (element->reflected ? 1 : 0) << " 0</mirrored>\n";// <<
                //"<color>" << rgbhex.ToCStr ().Get () << "</color>\n";
            }
                break;
                
            default:
                break;
        }

        oChannel << "</object>\n";
    }


    
    /*
     oChannel << "Object (guid:" << APIGuid2GSGuid (element->head.guid).ToUniString ().ToCStr () <<
     " begC:(" << element->begC.x << ", " << element->begC.y << ")" <<
     " endC:(" << element->endC.x << ", " << element->endC.y << ")" <<
     " color:(R" << static_cast<int> (pen.rgb.f_red * 255.0) <<
     ", G" << static_cast<int> (pen.rgb.f_green * 255.0) <<
     ", B" << static_cast<int> (pen.rgb.f_blue * 255.0) << "))\n";
     */
    
}

void ObjectAdapter::operator>> (GS::OChannel& oChannel)
{
    Output (oChannel);
}
