#include "WindowDoorAdapter.hpp"
#include "IAttributeProxy.hpp"
#include "APICommon.h"


WindowDoorAdapter::WindowDoorAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory) :
	GSAPI::IAPIOutputAdapter<API_WindowType> (manager, factory)
{
}

WindowDoorAdapter::WindowDoorAdapter (const WindowDoorAdapter& adapter) :
	GSAPI::IAPIOutputAdapter<API_WindowType> (adapter)
{
}

WindowDoorAdapter::~WindowDoorAdapter ()
{
}

WindowDoorAdapter& WindowDoorAdapter::operator= (const WindowDoorAdapter& adapter)
{
	static_cast<GSAPI::IAPIOutputAdapter<API_WindowType>&> (*this) = adapter;
	return (*this);
}

void WindowDoorAdapter::Output (GS::OChannel& oChannel)
{
	if ((DBERROR (outputManager == nullptr)) || (DBERROR (outputAdapterFactory == nullptr)) || (DBERROR (element == nullptr)) || (DBERROR (dbInfo == nullptr))) {
		return;
	}
    
    GSErrCode                   err           = NoError;
    GS::uchar_t         refid[API_UAddParStrLen];
    
    API_ElementMemo  memo;
    API_AddParType       **addPars = nullptr;
    
    err = ACAPI_Element_GetMemo (element->head.guid, &memo, API_DoorID);
    if (err == NoError) {
        addPars = memo.params;
        Int32                addParNum, i;
        
        addParNum = BMGetHandleSize (reinterpret_cast<GSHandle> (addPars)) / sizeof (API_AddParType);
        
        for (i = 0; i < addParNum; i++) {
            // get floorplanner refid from object
            if (CHEqualCStrings((*addPars)[i].name, "FP_id"))
            {
                BNCopyMemory(refid, (*addPars)[i].value.uStr, API_UAddParStrLen);
            }
        }
        /*
        oChannel <<
        " addPars: " << sizeof(addPars) <<
        " memo.params: " << sizeof(memo.params) <<
        " BMGetHandleSize: " << BMGetHandleSize (reinterpret_cast<GSHandle> (addPars)) / sizeof (API_AddParType) <<
        " *BMGetHandleSize: " << BMGetHandleSize (reinterpret_cast<GSHandle> (*addPars)) / sizeof (API_AddParType) <<
        " BMGetPtrSize addPars: " << BMGetPtrSize (reinterpret_cast<GSPtr>(memo.params)) / sizeof (API_AddParType) <<
        " API_AddParType: " << sizeof(API_AddParType) << "\n";
        
        int i = 0;
        oChannel <<
        "typeID: "      << (*addPars)[i].typeID <<
        " name: "       << (*addPars)[i].name <<
        " descname: "   << GS::UniString ((*addPars)[i].uDescname).ToCStr() <<
        " flags:"       << (*addPars)[i].flags <<
        " value.real:"  << (*addPars)[i].value.real <<
        " value.str:"   << GS::UniString ((*addPars)[i].value.uStr).ToCStr() <<
        "\n";*/
        
    }
    ACAPI_DisposeElemMemoHdls (&memo);
    
    /*
    
    double               a, b;
    Int32                addParNum, ind, i1, i2;
    API_AddParType       **addPars = nullptr;
    double               value;
    char                 *valueStr;
    
    GS::uchar_t         refid[API_UAddParStrLen];
    double height = 0;
    
    err = ACAPI_LibPart_GetParams (element->head., &a, &b, &addParNum, &addPars);
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
    }
    */
    
    
	const API_PenType& pen = attributeProxy->GetProxiedAttribute (*dbInfo, API_PenID, element->openingBase.sectContPen).pen;
	oChannel <<
    "<object>\n";
/*
    oChannel << "element->dirVector.x: " << element->dirVector.x << " element->dirVector.y: " << element->dirVector.y << "\n";
    oChannel <<
        " reflected: " << element->openingBase.reflected <<
        " oSide: " << element->openingBase.oSide <<
        " refSide: " << element->openingBase.refSide << "\n";
*/
	if (element->head.typeID == API_WindowID){
        
        if (refid[0] == '\0') {
            BNCopyMemory(refid, "e919", 5);
        }
        
		oChannel <<
        "<asset refid=\"" << GS::UniString (refid).ToCStr() << "\"/>\n" <<
        "<type>window";
	} else {
        
        if (refid[0] == '\0') {
            BNCopyMemory(refid, "e1826", 6);
        }
        
		oChannel <<
        "<asset refid=\"" << GS::UniString (refid).ToCStr() << "\"/>\n" <<
        "<type>door";
	}


 /*
	oChannel << "guid:" << APIGuid2GSGuid (element->head.guid).ToUniString ().ToCStr () << "\n" <<
				" width:" << element->openingBase.width << "\n" <<
				" height:" << element->openingBase.height << "\n" <<
                " lower:" << element->lower << "\n" <<
				" color:(R" << static_cast<int> (pen.rgb.f_red * 255.0) <<
				", G" << static_cast<int> (pen.rgb.f_green * 255.0) <<
				", B" << static_cast<int> (pen.rgb.f_blue * 255.0) << "))\n" <<
                " location:" << element->objLoc << "\n" <<
                " revealDepthOffset: " << element->revealDepthOffset << "\n" <<
                " revealDepthFromSide: " << element->revealDepthFromSide << "\n" <<
                " startPoint: " << element->startPoint.x << " " << element->startPoint.y << "\n" <<
                " dirVector: " << element->dirVector.x << " " << element->dirVector.y << "\n" <<
                " fixPoint: " << element->fixPoint << "\n";
 */
    API_Element    selElem;
    
    BNZeroMemory (&selElem, sizeof (selElem));
    selElem.header.guid = element->owner;
    err = ACAPI_Element_Get (&selElem);
 //   if (err != NoError) {
 //       return err;
 //   }
//    oChannel << "selElem: " << selElem.wall.begC.x << " " << selElem.wall.begC.y << "\n";
    
    API_Coord begC, endC;
    begC.x = selElem.wall.begC.x;
    begC.y = -selElem.wall.begC.y;
    endC.x = selElem.wall.endC.x;
    endC.y = -selElem.wall.endC.y;
    
    API_Vector vecU;
    
    double vecLen = sqrt(pow(endC.x-begC.x, 2) + pow(endC.y-begC.y,2));
    // TODO: check divide by zero
    vecU.x = (endC.x-begC.x) / vecLen;
    vecU.y = (endC.y-begC.y) / vecLen;
    
    API_Coord wdC;
    
    wdC.x = vecU.x * element->objLoc + begC.x;
    wdC.y = vecU.y * element->objLoc + begC.y;
    
//    oChannel << "wdC: " << wdC.x << " " << wdC.y << "\n";
    //a = atan2d(x1*y2-y1*x2,x1*x2+y1*y2); // from web
    
    // rotation of the object releted to horizontal unit vector -> x = 1
    double aRad = atan2(1 * vecU.y - 0 * vecU.x, 1 * vecU.x + 0 * vecU.y);
    double aDeg = aRad * RADDEG;
    
    oChannel <<
    "</type>\n" <<
    "<points>" << wdC.x << " " << wdC.y << " " << selElem.wall.bottomOffset + element->lower << "</points>\n" <<
    "<size>" << element->openingBase.width << " " << selElem.wall.thickness << " " << element->openingBase.height << "</size>\n" <<
    "<rotation>0 0 " << aDeg << "</rotation>\n";
    
    if (element->openingBase.oSide && element->openingBase.reflected) {
        oChannel << "<mirrored>0 0 0</mirrored>\n";
    }
    else if (element->openingBase.oSide && !element->openingBase.reflected) {
        oChannel << "<mirrored>1 0 0</mirrored>\n";
    }
    else if (!element->openingBase.oSide && !element->openingBase.reflected) {
        oChannel << "<mirrored>0 1 0</mirrored>\n";
    }
    else if (!element->openingBase.oSide && element->openingBase.reflected) {
        oChannel << "<mirrored>1 1 0</mirrored>\n";
    }
    oChannel << "</object>\n";
    
    
}

void WindowDoorAdapter::operator>> (GS::OChannel& oChannel)
{
	Output (oChannel);
}
