#include "RoofAdapter.hpp"
#include "IAttributeProxy.hpp"
#include "APICommon.h"

RoofAdapter::RoofAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory) :
GSAPI::IAPIOutputAdapter<API_RoofType> (manager, factory)
{
}

RoofAdapter::RoofAdapter (const RoofAdapter& adapter) :
GSAPI::IAPIOutputAdapter<API_RoofType> (adapter)
{
}

RoofAdapter::~RoofAdapter ()
{
}

RoofAdapter& RoofAdapter::operator= (const RoofAdapter& adapter)
{
    static_cast<GSAPI::IAPIOutputAdapter<API_RoofType>&> (*this) = adapter;
    return (*this);
}

// get height of roof points from base line plane
// get distance from point to line
// Line defined by two points
// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
double roofZ (API_Sector baseLine, double angle, API_Coord point)
{
    API_Coord c0 = point;
    API_Coord c1 = baseLine.c1;
    API_Coord c2 = baseLine.c2;
    
    double dist = abs((c2.y-c1.y) * c0.x - (c2.x - c1.x) * c0.y + c2.x * c1.y - c2.y * c1.x)/
        sqrt(pow(c2.y - c1.y, 2) + pow(c2.x - c1.x, 2));
    return dist * tan(angle);
}

void RoofAdapter::Output (GS::OChannel& oChannel)
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
     <area>
     <asset refid="rs-13551"/>
     <type>custom_area</type>
     <layer>2</layer>
     <color>#CCCCCC</color>
     <points>8.994 -13.103 0.000 10.999 -13.103 1.500,10.999 -13.103 1.500 10.999 -12.007 1.500,10.999 -12.007 1.500 8.994 -12.007 0.000,8.994 -12.007 0.000 8.994 -13.103 0.000</points>
     <rotation>0 0 0</rotation>
     </area>
     */
    
    switch (element->roofClass) {
            
        case API_PlaneRoofID:
        {
            int         i, begInd, endInd;
            API_Coord   begC, endC;
            double heightMin;
            double pivotOffset = 0;
            double storyLevel = 0;
            GSErrCode        err;
            
            API_StoryInfo    storyInfo;
            err = ACAPI_Environment (APIEnv_GetStorySettingsID, &storyInfo, nullptr);
            if (err != NoError) {
                return;
            }
            
            Int32 n = storyInfo.lastStory - storyInfo.firstStory;
            
            //oChannel << "element->head.floorInd: " << element->head.floorInd << "\n";
            
            for (Int32 i = storyInfo.firstStory; i <= n; i++) {
                //oChannel << i << " floorId: " << (*storyInfo.data)[i].floorId <<
                //    " index: " << (*storyInfo.data)[i].index <<
                 //   " level: " << (*storyInfo.data)[i].level <<
                 //   "\n";
                
                if ((*storyInfo.data)[i].index == element->head.floorInd)
                {
                    storyLevel = (*storyInfo.data)[i].level;
                }
            }
            
            begInd = (*memo.pends) [1-1] + 1;
            endInd = (*memo.pends) [1];
            
            // get the lowest point of the roof from base line
            
            heightMin = roofZ(element->u.planeRoof.baseLine, element->u.planeRoof.angle, (*memo.coords) [begInd]);
            
            for (i = begInd; i < endInd; i++) {
                double height = roofZ(element->u.planeRoof.baseLine, element->u.planeRoof.angle, (*memo.coords) [i]);
                //oChannel << "h" << i << " height: " << height << "\n";
                if (heightMin > height)
                {
                    heightMin = height;
                }
            }
            
            //oChannel << "heightMin: " << heightMin << "\n";
            
            // get lowest point from home story
            API_ElemInfo3D          info3D;
            err = ACAPI_Element_Get3DInfo(element->head, &info3D);
            if (err == NoError) {
                pivotOffset = info3D.bounds.zMin - heightMin - storyLevel;
            }
            
            //oChannel << "pivotOffset: " << pivotOffset << "\n";
            
            //oChannel << "info3D.bounds.zMin: " << info3D.bounds.zMin << "\n";
            
            API_Vector vecBL;
            // TODO: check divide by zero
            vecBL.x = element->u.planeRoof.baseLine.c2.x-element->u.planeRoof.baseLine.c1.x;
            vecBL.y = element->u.planeRoof.baseLine.c2.y-element->u.planeRoof.baseLine.c1.y;
            
            // angle of the wall releted to horizontal unit vector -> x = 1
            double aRad = atan2(1 * vecBL.y - 0 * vecBL.x, 1 * vecBL.x + 0 * vecBL.y);
            double aDeg = aRad * RADDEG;
            
            //oChannel << "posSign: " << element->u.planeRoof.posSign << "\n";
            
            oChannel << "<area>\n";
            oChannel << "<asset refid=\"rs-13551\"/>\n";
            oChannel << "<type>custom_area</type>\n";
            oChannel << "<layer>2</layer>\n";
            oChannel << "<points>";
            
            for (i = begInd; i < endInd; i++) {
                begC = (*memo.coords) [i];
                endC = (*memo.coords) [i+1];
                
                oChannel <<
                begC.x << " " << -begC.y << " " <<
                    roofZ(element->u.planeRoof.baseLine, element->u.planeRoof.angle, begC) + pivotOffset << " " <<
                endC.x << " " << -endC.y << " " <<
                    roofZ(element->u.planeRoof.baseLine, element->u.planeRoof.angle, endC) + pivotOffset;
                
                // add comma separator after each segment coordinates
                if (i != endInd - 1)
                {
                    oChannel << ",";
                }
            }
            oChannel << "</points>\n" <<
            "<rotation>0 0 " << 180 - aDeg << "</rotation>\n" <<
            "</area>\n";
            
        }
            break;
            
        case API_PolyRoofID:
        {
            oChannel << "API_PolyRoofID\n";
        }
            break;
            
        default:
            break;
            
    }
    

}

void RoofAdapter::operator>> (GS::OChannel& oChannel)
{
    Output (oChannel);
}
