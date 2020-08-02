//
//  FMLFileOutputManager.hpp
//  ExportToFML
//
//  Created by Nastya on 07.12.2019.
//

#ifndef FMLFileOutputManager_hpp
#define FMLFileOutputManager_hpp

#include "FileOutputManager.hpp"
#include "TextOProtocol.hpp"


class FMLFileOutputManager : public GSAPI::FileOutputManager
{
private:
    FMLFileOutputManager (const FMLFileOutputManager& manager); // disabled
    FMLFileOutputManager& operator= (const FMLFileOutputManager& manager);  // disabled
    
public:
    FMLFileOutputManager (const IO::Location& location) : GSAPI::FileOutputManager (location) { outFile.SetOutputProtocol (GS::textOProtocol); }
    virtual ~FMLFileOutputManager (void);
    short   floorInd;
};

#endif /* FMLFileOutputManager_hpp */
