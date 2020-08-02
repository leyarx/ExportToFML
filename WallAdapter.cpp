#include "WallAdapter.hpp"
#include "IAttributeProxy.hpp"
#include "APICommon.h"


WallAdapter::WallAdapter (GSAPI::IAPIOutputManager* manager, GSAPI::IAPIOutputAdapterFactory* factory) :
	GSAPI::IAPIOutputAdapter<API_WallType> (manager, factory)
{
}

WallAdapter::WallAdapter (const WallAdapter& adapter) :
	GSAPI::IAPIOutputAdapter<API_WallType> (adapter)
{
}

WallAdapter::~WallAdapter ()
{
}

WallAdapter& WallAdapter::operator= (const WallAdapter& adapter)
{
	static_cast<GSAPI::IAPIOutputAdapter<API_WallType>&> (*this) = adapter;
	return (*this);
}


//
bool getZ (API_Gable g, double length, API_Coord3D& o, GS::OChannel& oChannel)
{
    double l1, l2, lmin, lmax;
    API_Vector origX, begL, endL;
    origX.x = 1;
    origX.y = 0;
    
    begL.x = g.nx * g.xb;
    begL.y = g.ny * g.xb;

    endL.x = g.nx * g.xe;
    endL.y = g.ny * g.xe;

     // get the intersection distance along wall refline from origin to vertical limit plane
    
    l1 = abs(g.xb) / ((origX.x * begL.x + origX.y * begL.y) /
                      (sqrt(pow(origX.x, 2) + pow(origX.y, 2)) * sqrt(pow(begL.x,2) + pow(begL.y,2))));
    l2 = abs(g.xe) / ((origX.x * endL.x + origX.y * endL.y) /
                      (sqrt(pow(origX.x, 2) + pow(origX.y, 2)) * sqrt(pow(endL.x,2) + pow(endL.y,2))));
    // sort limits
    lmin = l1 <= l2 ? l1 : l2;
    lmax = l1 >= l2 ? l1 : l2;
    
    
    // get angle between vectors
//    oChannel << "angle: " << RADDEG * acos((origX.x * begL.x + origX.y * begL.y) /
//        (sqrt(pow(origX.x, 2) + pow(origX.y, 2)) * sqrt(pow(begL.x,2) + pow(begL.y,2)))) << "\n";
    
    // get the intersection distance along wall refline from origin to vertical limit plane
//    oChannel << "beg length: " << abs(g.xb) / ((origX.x * begL.x + origX.y * begL.y) /
//                                      (sqrt(pow(origX.x, 2) + pow(origX.y, 2)) * sqrt(pow(begL.x,2) + pow(begL.y,2)))) << "\n";
//    oChannel << "end length: " << abs(g.xe) / ((origX.x * endL.x + origX.y * endL.y) /
//                                               (sqrt(pow(origX.x, 2) + pow(origX.y, 2)) * sqrt(pow(endL.x,2) + pow(endL.y,2)))) << "\n";
    
/*
    // check if the point is lay in limits
    if (length < lmin or length > lmax)
    {
        return false;
    }
*/
    // the point on the refline
    o.x = length;
    
    // the point is always lay on wall ref line
    o.y = 0;
    // get z-value of the plain in specified x,y coordinates;
 
    o.z = (g.a * o.x + g.b * o.y - g.d) / -g.c;
    
    return true;
}

double getPosOnRefline (double nx, double ny, double len)
{
    API_Vector origX, begL;
    
    origX.x = 1;
    origX.y = 0;
    begL.x = nx * len;
    begL.y = ny * len;
    
    return abs(len) / ((origX.x * begL.x + origX.y * begL.y) /
                    (sqrt(pow(origX.x, 2) + pow(origX.y, 2)) * sqrt(pow(begL.x,2) + pow(begL.y,2))));
}

double getPositionZ (API_Gable g, double pos)
{
    // get z-value of the plain in specified x = pos, y=0 coordinates;
    return (g.a * pos + g.b * 0 - g.d) / -g.c;
}

double getPositionX (API_Gable g, double height)
{
    // get z-value of the plain in specified z = height, y=0 coordinates;
    return (g.d - g.b * 0 - g.c * height) / g.a;
}

// get the intersection point of gable limits with wall refline and check pos to be in limits
bool checkPosGableLimit(API_Gable g, double pos)
{
    double l1, l2, lmin, lmax;
    API_Vector origX, begL, endL;
    origX.x = 1;
    origX.y = 0;
    
    begL.x = g.nx * g.xb;
    begL.y = g.ny * g.xb;
    
    endL.x = g.nx * g.xe;
    endL.y = g.ny * g.xe;
    
    // get the intersection distance along wall refline from origin to vertical limit plane
    
    l1 = abs(g.xb) / ((origX.x * begL.x + origX.y * begL.y) /
                      (sqrt(pow(origX.x, 2) + pow(origX.y, 2)) * sqrt(pow(begL.x,2) + pow(begL.y,2))));
    l2 = abs(g.xe) / ((origX.x * endL.x + origX.y * endL.y) /
                      (sqrt(pow(origX.x, 2) + pow(origX.y, 2)) * sqrt(pow(endL.x,2) + pow(endL.y,2))));
    // sort limits
    lmin = l1 <= l2 ? l1 : l2;
    lmax = l1 >= l2 ? l1 : l2;
    
    // check if the point is lay in limits
    if (pos < lmin or pos > lmax)
    {
        return false;
    }
    
    return true;
}

// convert point in local coordinate system to world coordinate system
// http://mathhelpplanet.com/static.php?p=pryeobrazovaniya-pryamougolnyh-koordinat
API_Coord3D localPointToWCS (API_Coord origin, double angle, API_Coord3D point)
{
    API_Coord3D res;
    
    res.x = origin.x + point.x * cos(angle) - point.y * sin(angle);
    res.y = origin.y + point.x * sin(angle) + point.y * cos(angle);
    res.z = point.z;
    return res;
}

void WallAdapter::Output (GS::OChannel& oChannel)
{
	if ((DBERROR (outputManager == nullptr)) || (DBERROR (outputAdapterFactory == nullptr)) || (DBERROR (element == nullptr)) || (DBERROR (dbInfo == nullptr))) {
		return;
	}

	const API_PenType& pen = attributeProxy->GetProxiedAttribute (*dbInfo, API_PenID, element->contPen).pen;

    API_ElementMemo memo;
    GSErrCode        err;
    Int32            nGable = 0, nCoords, i, j, k;
    
    BNZeroMemory (&memo, sizeof (API_ElementMemo));
    err = ACAPI_Element_GetMemo (element->head.guid, &memo, APIMemoMask_Gables);
    
    if (err != NoError) {
        return;
    }
    
    //oChannel << "flipped: " << element->flipped << "\n";
    
    if (memo.gables != nullptr) {
        nGable = BMGetHandleSize ((GSHandle) memo.gables) / sizeof (API_Gable);
    }
    
//oChannel << "nGable: " << nGable << "\n";
    
    if (nGable > 0) {
        
        double length;
        
        GS::Array<API_Coord3D> coords, coordsTemp, coordsTempSorted;
        
        // wall length
        length = sqrt(pow(element->endC.x - element->begC.x, 2) + pow(element->endC.y - element->begC.y, 2));
        
        
//        double a1, b1, c1, d1, a2, b2, c2, d2, z, x, x0, y0, z0, x1, y1, z1;
//        a1 = (*memo.gables)[0].a;
//        b1 = (*memo.gables)[0].b;
//        c1 = (*memo.gables)[0].c;
//        d1 = (*memo.gables)[0].d;
//        a2 = (*memo.gables)[1].a;
//        b2 = (*memo.gables)[1].b;
//        c2 = (*memo.gables)[1].c;
//        d2 = (*memo.gables)[1].d;
        
        
//        x = (c1 * (-d2) - c2 * (-d1)) / (a1 * c2 - a2 * c1);
//        z = (a2 * (-d1) - a1 * (-d2)) / (a1 * c2 - a2 * c1);
        
        //z = (-d2 * a1 - a2 * (-d1))/(-a2 * (-c1) + c2 * a1);
        
 //       oChannel << "x: " << x << " z: " << z << "\n";
        
        
//        x0 = 0;
//        y0 = 0;
//        z0 = (a2 * x0 + b2 * y0 - d2) / -c2;
        
 //       oChannel << "x0: " << x0 << " z0: " << z0 << "\n";
        
//        x1 = length;
//        y1 = 0;
//        z1 = (a2 * x1 + b2 * y1 - d2) / -c2;
        
//        oChannel << "x1: " << x1 << " z1: " << z1 << "\n";
    
      
        // begin top coordinate world
        API_Coord3D begT;
        
        begT.x = element->begC.x;
        begT.y = element->begC.y;
        begT.z = element->height;
        
        for (i = 0; i < nGable; i++) {
            API_Coord3D o;
            
            //oChannel << "g" << i << "\n";
            //oChannel << getZ((*memo.gables)[i], 0, o, oChannel) << " z: " << o.z << "\n";
            
            if (getZ((*memo.gables)[i], 0, o, oChannel))
            {
                if (o.z < begT.z)
                    begT.z = o.z;
            }
        }

        // end top coordinate world
        API_Coord3D endT;
        
        endT.x = element->endC.x;
        endT.y = element->endC.y;
        endT.z = element->height;
        
        for (i = 0; i < nGable; i++) {
            API_Coord3D o;
            
            //oChannel << "g" << i << "\n";
            //oChannel << getZ((*memo.gables)[i], length, o, oChannel) << " z: " << o.z << "\n";
            
            if (getZ((*memo.gables)[i], length, o, oChannel))
            {
                if (o.z < endT.z)
                    endT.z = o.z;
            }
        }
 
        
        //
        // loop over each gable and add points of gable limits to points array (checking for wall limits)
        //
        //oChannel << "wall length:" << length << "\n";
        
        for (i = 0; i < nGable; i++) {
            double xbRef, xeRef, xbRefZ, xeRefZ;
            //oChannel << "g_" << i << ":\n";
            //oChannel << "nx:" << (*memo.gables)[i].nx << "\n";
            //oChannel << "ny:" << (*memo.gables)[i].ny << "\n";
            //oChannel << "xb:" << (*memo.gables)[i].xb << "\n";
            //oChannel << "xe:" << (*memo.gables)[i].xe << "\n";
            
            xbRef = getPosOnRefline((*memo.gables)[i].nx, (*memo.gables)[i].ny, (*memo.gables)[i].xb);
            
            //oChannel << "xbRef: " << xbRef << "\n";
            if (xbRef > 0 and xbRef < length)
            {
                xbRefZ = getPositionZ((*memo.gables)[i], xbRef);
                //oChannel << "xbRefZ: " << xbRefZ << "\n";
                if (xbRefZ > element->bottomOffset and xbRefZ < element->height)
                {
                    // add point to points array
                    //oChannel << "xbRef add\n";
                    API_Coord3D cT;
                    cT.x = xbRef;
                    cT.y = 0;
                    cT.z = xbRefZ;
                    coordsTemp.Push(cT);
                }
            }
            
            xeRef = getPosOnRefline((*memo.gables)[i].nx, (*memo.gables)[i].ny, (*memo.gables)[i].xe);
            
            //oChannel << "xeRef: " << xeRef << "\n";
            if (xeRef > 0 and xeRef < length)
            {
                xeRefZ = getPositionZ((*memo.gables)[i], xeRef);
                //oChannel << "xeRefZ: " << xeRefZ << "\n";
                if (xeRefZ > element->bottomOffset and xeRefZ < element->height)
                {
                    // add point to points array
                    //oChannel << "xeRef add\n";
                    
                    API_Coord3D cT;
                    cT.x = xeRef;
                    cT.y = 0;
                    cT.z = xeRefZ;
                    coordsTemp.Push(cT);

                }
            }
      
        }
        
        //
        // intersect height of the wall with each gable
        for (i = 0; i < nGable; i++) {
            double hx;
            hx = getPositionX ((*memo.gables)[i], element->height);
            
            // check position for wall limits
            if (hx > 0 and hx < length)
            {
                // check position for gable limits
                //oChannel << "checkPosGableLimit: " << checkPosGableLimit((*memo.gables)[i], hx) << "\n";
                // if pos in limits add to points array
                if(checkPosGableLimit((*memo.gables)[i], hx))
                {
                    //oChannel << "hx add\n";
                    
                    API_Coord3D cT;
                    cT.x = hx;
                    cT.y = 0;
                    cT.z = element->height;
                    coordsTemp.Push(cT);
                }
            }
        }

        // change coordinate systems
        API_Vector vecU;
        // TODO: check divide by zero
        vecU.x = (element->endC.x-element->begC.x) / length;
        vecU.y = (element->endC.y-element->begC.y) / length;
        
        //    oChannel << "wdC: " << wdC.x << " " << wdC.y << "\n";
        //a = atan2d(x1*y2-y1*x2,x1*x2+y1*y2); // from web
        
        // angle of the wall releted to horizontal unit vector -> x = 1
        double aRad = atan2(1 * vecU.y - 0 * vecU.x, 1 * vecU.x + 0 * vecU.y);
//        double aDeg = aRad * RADDEG;
        
        // add begin point to array
        coords.Push(begT);
        // sort coordsTemp array
        // coordsTempSorted
        while (!coordsTemp.IsEmpty())
        {
            double tempX = coordsTemp[0].x;
            int tempIndex = 0;
            
            for (i = 1; i < coordsTemp.GetSize(); i++)
            {
                if (tempX > coordsTemp[i].x)
                {
                    tempX = coordsTemp[i].x;
                    tempIndex = i;
                }
            }
            //coordsTempSorted.Push(coordsTemp[tempIndex]);
            //oChannel << "point x: " << coordsTemp[tempIndex].x << " y:" << coordsTemp[tempIndex].y << " z:" << coordsTemp[tempIndex].z << "\n";
            
            // check point if it is near begin or end of the wall and skip it if so
            // also check z value
            if ((coordsTemp[tempIndex].x > 0.02 and coordsTemp[tempIndex].x < length - 0.02) or
                    (coordsTemp[tempIndex].x < 0.02 and abs(coordsTemp[tempIndex].z-begT.z) > 0.1) or
                    (coordsTemp[tempIndex].x > length - 0.02 and abs(coordsTemp[tempIndex].z-endT.z) > 0.1))
            {
                coords.Push(localPointToWCS(element->begC, aRad, coordsTemp[tempIndex]));
            }

            coordsTemp.Delete(tempIndex);
        }
        // add end point to array
        coords.Push(endT);
        
        //oChannel << "coords size: " << coords.GetSize() << "\n";
        for (i = 0; i < coords.GetSize()-1; i++)
        {
            oChannel << "<line>\n" <<
            "<asset refid=\"l1\"/>\n" <<
            "<type>default_wall</type>\n" <<
            "<points>" <<
            coords[i].x << " " << coords[i].y * -1 << " " << element->bottomOffset << " " <<
            coords[i+1].x << " " << coords[i+1].y * -1 << " " << element->bottomOffset << "," <<
            coords[i].x << " " << coords[i].y * -1 << " " << element->bottomOffset+coords[i].z << " " <<
            coords[i+1].x << " " << coords[i+1].y * -1 << " " << element->bottomOffset+coords[i+1].z << "</points>\n" <<
            "<thickness>" << element->thickness << "</thickness>\n";
            
            oChannel << "<balance>";
            switch (element->referenceLineLocation)
            {
                case APIWallRefLine_Outside:
                    oChannel << (element->flipped ? "1" : "0");
                    break;
                case APIWallRefLine_Inside:
                    oChannel << (element->flipped ? "0" : "1");
                    break;
                case APIWallRefLine_Center:
                default:
                    oChannel << "0.5";
                    break;
            }
            oChannel << "</balance>\n";
            
            oChannel << "<color>#000000</color>\n" <<
            "</line>\n";
        }
        
    }
    else
    {
    /*
    for (int i = (*memo.pends) [1-1] + 1; i < (*memo.pends) [1]; i++) {
        oChannel <<
        (*memo.coords) [i].x << " " << (*memo.coords) [i].y << "\n";
    }
    */
    oChannel << "<line>\n" <<
                "<asset refid=\"l1\"/>\n" <<
                "<type>default_wall</type>\n" <<
                "<points>" <<
                    element->begC.x << " " << element->begC.y * -1 << " " << element->bottomOffset << " " <<
                    element->endC.x << " " << element->endC.y * -1 << " " << element->bottomOffset << "," <<
                    element->begC.x << " " << element->begC.y * -1 << " " << element->bottomOffset+element->height << " " <<
                    element->endC.x << " " << element->endC.y * -1 << " " << element->bottomOffset+element->height << "</points>\n" <<
        "<thickness>" << element->thickness << "</thickness>\n";
        
        oChannel << "<balance>";
        switch (element->referenceLineLocation)
        {
            case APIWallRefLine_Outside:
                oChannel << (element->flipped ? "1" : "0");
                break;
            case APIWallRefLine_Inside:
                oChannel << (element->flipped ? "0" : "1");
                break;
            case APIWallRefLine_Center:
            default:
                oChannel << "0.5";
                break;
        }
        oChannel << "</balance>\n";
        
        oChannel << "<color>#000000</color>\n" <<
        "</line>\n";
/*
    oChannel << "Wall (guid:" << APIGuid2GSGuid (element->head.guid).ToUniString ().ToCStr () <<
    " begC:(" << element->begC.x << ", " << element->begC.y << ")" <<
    " endC:(" << element->endC.x << ", " << element->endC.y << ")" <<
    " color:(R" << static_cast<int> (pen.rgb.f_red * 255.0) <<
    ", G" << static_cast<int> (pen.rgb.f_green * 255.0) <<
    ", B" << static_cast<int> (pen.rgb.f_blue * 255.0) << "))\n";
*/
    }
}

void WallAdapter::operator>> (GS::OChannel& oChannel)
{
	Output (oChannel);
}
