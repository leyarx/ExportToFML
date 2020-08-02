/**
 * @file Contains the functions required by ARCHICAD.
 */

#include	"APIEnvir.h"
#include    "Array.hpp"
#include    "BFSTreeSelector.hpp"
#include    "FMLFileOutputManager.hpp"
#include    "APIOutputApplication.hpp"
#include    "DrawOrderElementContainer.hpp"
#include    "IndexedElementContainer.hpp"

#include    "ElementIterator.hpp"
#include    "FMLOutputAdapterFactory.hpp"
#include    "AttributeProxy.hpp"
#include    "NullTypeAttributeAdapterFactory.hpp"

#include    "APIdefs_Environment.h"

enum {
    FILE_TYPE_MENU_ID        = 32500,
};

/**
 * Dependency definitions.
 * @param envir [in] ARCHICAD environment values.
 * @return The Add-On loading type.
 */
API_AddonType	__ACENV_CALL	CheckEnvironment (API_EnvirParams* envir)
{
	RSGetIndString (&envir->addOnInfo.name       , 32000, 1, ACAPI_GetOwnResModule ());
	RSGetIndString (&envir->addOnInfo.description, 32000, 2, ACAPI_GetOwnResModule ());

	return APIAddon_Normal;
}		// CheckEnvironment ()

/**
 * Interface definitions.
 * @return ARCHICAD error code.
 */
GSErrCode	__ACENV_CALL	RegisterInterface (void)
{
    GSErrCode err = ACAPI_Register_FileType (1, 'TEXT', 0, "fml", 0, FILE_TYPE_MENU_ID, 1, SaveAs2DSupported);

	return err;
}		// RegisterInterface

/**
 * Save as command handler function. Function name is NOT fixed. There can be
 * more than one of these functions. Please check the API Development Kit
 * documentation for more information.
 *
 * @param ioParams [in] Parameters of the menu command.
 * @return ARCHICAD error code.
 */
GSErrCode __ACENV_CALL    IOCommandHandler (const API_IOParams* ioParams)
{
    GSErrCode        err;
    
    // get projecy info
    API_ProjectInfo        projectInfo;
    
    err = ACAPI_Environment (APIEnv_ProjectID, &projectInfo, nullptr);
    if (err != NoError) {
        return err;
    }
    
    GS::Array<API_DatabaseInfo> floorPlanArray;
    floorPlanArray.Push (GSAPI::DatabaseGraph::Instance ().SelectDatabases (GSAPI::BFSTreeSelector ())[0]);
    //floorPlanArray.Push (GSAPI::DatabaseGraph::Instance ().SelectDatabases (GSAPI::BFSTreeSelector ())[9]);
    //floorPlanArray = GSAPI::DatabaseGraph::Instance ().SelectDatabases (GSAPI::BFSTreeSelector ());
    
    // we need output only on the floor plan
    GSAPI::BFSTreeSelector selector (floorPlanArray);
    FMLFileOutputManager outputManager (*ioParams->fileLoc);

//    outputManager.floorInd = 9;

    
    // just strange piece of code to get outfile
    API_Elem_Head e;
    GS::OChannel& oChannel = outputManager.GetOChannel(e, floorPlanArray[0]);
    
    oChannel <<
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<project>\n"
    "   <name>";
    
    // get archicad filename
    if (projectInfo.untitled)
        oChannel << "untitled";
    else {
        oChannel << projectInfo.location->ToDisplayText ().ToCStr ().Get ();
    }
    
    oChannel << "</name>\n"
    "   <floors type=\"array\">\n";
    
    

    
/*
    USize dbCount = floorPlanArray.GetSize ();
    for (USize i = 0; i < dbCount; i++) {
        oChannel << "typeId: " << floorPlanArray[i].typeID << "\n" <<
            "Title: " << GS::UniString (floorPlanArray[i].title).ToCStr ().Get () << "\n" <<
            "name: " << GS::UniString (floorPlanArray[i].name).ToCStr ().Get () << "\n" <<
            "ref: " << GS::UniString (floorPlanArray[i].ref).ToCStr ().Get () << "\n";
        oChannel << "\n";
    }

    oChannel << "lol lol\n";
*/
    API_ElemFilter fmlElemFilter;
    BNZeroMemory (&fmlElemFilter, sizeof (fmlElemFilter));
    //fmlElemFilter[API_WallID] = true;
    //fmlElemFilter[API_WindowID] = true;
    //fmlElemFilter[API_DoorID] = true;
    
    // we want only the pen colors
    GSAPI::AttributeFilter fmlAttributeFilter;
    BNZeroMemory (&fmlAttributeFilter, sizeof (fmlAttributeFilter));
    fmlAttributeFilter[API_PenID] = true;
    
    GSAPI::APIOutputApplication outputApplication;
    // we want to output the pen color attributes inline during element output, so no Attribute Adapter is needed -> use NullTypeAttributeAdapterFactory
    
    API_StoryInfo    storyInfo;
    err = ACAPI_Environment (APIEnv_GetStorySettingsID, &storyInfo, nullptr);
    if (err != NoError) {
        return err;
    }
    
    Int32 n = storyInfo.lastStory - storyInfo.firstStory;
    bool exported = false;
    //GS::uchar_t     storyName [API_UniLongNameLen];
    
    //oChannel << "storyInfo.lastStory:" << storyInfo.lastStory << "\n";
    //oChannel << "storyInfo.firstStory:" << storyInfo.firstStory << "\n";
    //oChannel << "n:" << n << "\n";
    
    for (Int32 i = 0; i <= n; i++) {
        
        GS::UniString storyName ((*storyInfo.data)[i].uName);
        
        if ((*storyInfo.data)[i].uName[0] == '-')
        {
            continue;
        }
        
        if ((*storyInfo.data)[i].uName[0] == '+' and exported)
        {
            storyName.DeleteFirst();
            oChannel <<
            "            </design>\n" <<
            "            <design>\n" <<
            "            <name>" << storyName.ToCStr ().Get () << "</name>\n" <<
            "            <units>meters</units>\n";
        }
        else
        {
            if (exported)
            {
                oChannel <<
                "            </design>\n" <<
                "        </designs>\n" <<
                "    </floor>\n";
            }
        
            exported = true;
        
        oChannel <<
        "    <floor>\n"
        "        <name>" << storyName.ToCStr ().Get () << "</name>\n" <<
        "        <level type=\"integer\">" << (*storyInfo.data)[i].level << "</level>\n" <<
        "        <height type=\"decimal\">" << ((*storyInfo.data)[i+1].level - (*storyInfo.data)[i].level) << "</height>\n" <<
        "        <designs type=\"array\">\n" <<
        "            <design>\n" <<
        "            <name>" << storyName.ToCStr ().Get () << "</name>\n" <<
        "            <units>meters</units>\n";
        }
        
        outputManager.floorInd = (*storyInfo.data)[i].index;

        fmlElemFilter[API_WallID] = true;
        fmlElemFilter[API_DimensionID] = true;
        fmlElemFilter[API_LineID] = true;

        oChannel << "<lines type=\"array\">\n";
        
        err = outputApplication.OutputElements<GSAPI::IndexedElementContainer<GSAPI::ElementIterator>,
        FMLOutputAdapterFactory,
        GSAPI::AttributeProxy<GSAPI::BFSTreeSelector>,
        GSAPI::NullTypeAttributeAdapterFactory> (outputManager, fmlElemFilter, selector, fmlAttributeFilter, GSAPI::InlineOutput_Index);
        
        oChannel << "</lines>\n";
        fmlElemFilter[API_WallID] = false;
        fmlElemFilter[API_DimensionID] = false;
        fmlElemFilter[API_LineID] = false;
        
        fmlElemFilter[API_WindowID] = true;
        fmlElemFilter[API_DoorID] = true;
        fmlElemFilter[API_TextID] = true;
        fmlElemFilter[API_ObjectID] = true;
        
        oChannel << "<objects type=\"array\">\n";
        
        err = outputApplication.OutputElements<GSAPI::IndexedElementContainer<GSAPI::ElementIterator>,
        FMLOutputAdapterFactory,
        GSAPI::AttributeProxy<GSAPI::BFSTreeSelector>,
        GSAPI::NullTypeAttributeAdapterFactory> (outputManager, fmlElemFilter, selector, fmlAttributeFilter, GSAPI::InlineOutput_Index);
        
        oChannel << "</objects>\n";
        fmlElemFilter[API_WindowID] = false;
        fmlElemFilter[API_DoorID] = false;
        fmlElemFilter[API_TextID] = false;
        fmlElemFilter[API_ObjectID] = false;
        

        oChannel << "<areas type=\"array\">\n";
        
        fmlElemFilter[API_RoofID] = true;
        
        err = outputApplication.OutputElements<GSAPI::IndexedElementContainer<GSAPI::ElementIterator>,
        FMLOutputAdapterFactory,
        GSAPI::AttributeProxy<GSAPI::BFSTreeSelector>,
        GSAPI::NullTypeAttributeAdapterFactory> (outputManager, fmlElemFilter, selector, fmlAttributeFilter, GSAPI::InlineOutput_Index);

        fmlElemFilter[API_RoofID] = false;
        
        fmlElemFilter[API_ZoneID] = true;
        fmlAttributeFilter[API_ZoneCatID] = true;
        
        err = outputApplication.OutputElements<GSAPI::IndexedElementContainer<GSAPI::ElementIterator>,
        FMLOutputAdapterFactory,
        GSAPI::AttributeProxy<GSAPI::BFSTreeSelector>,
        GSAPI::NullTypeAttributeAdapterFactory> (outputManager, fmlElemFilter, selector, fmlAttributeFilter, GSAPI::InlineOutput_Index);
        
        fmlAttributeFilter[API_ZoneCatID] = false;
        fmlElemFilter[API_ZoneID] = false;
        
        oChannel << "</areas>\n";
        

    }
    
    oChannel <<
    "            </design>\n" <<
    "        </designs>\n" <<
    "    </floor>\n";
    
    oChannel <<
    "    </floors>\n"
    "</project>\n";
    
    return err;
}

/**
 * Called after the Add-On has been loaded into memory.
 * @return ARCHICAD error code.
 */
GSErrCode __ACENV_CALL	Initialize (void)
{
	GSErrCode err = NoError;

    ACAPI_Install_FileTypeHandler (1, IOCommandHandler);
    
	return err;
}		// Initialize

/**
 * Called when the Add-On is going to be unloaded.
 * @return ARCHICAD error code.
 */
GSErrCode __ACENV_CALL	FreeData (void)

{
	GSErrCode err = NoError;

	return err;
}		// FreeData
