#include <graphics/tilemap.h>
#include <utilities/json_utility.h>

namespace neko
{

void TiledMap::Init(const std::string &tilemapPath, Physics2dManager* physics2DManager)
{

    std::unique_ptr<json> tileMapJsonPtr;
    tileMapJsonPtr = LoadJson(tilemapPath);
    json tileMapJson = *tileMapJsonPtr;
    //TODO import the texture and fill the double buffer vertex arrays of tilesheets
    for(auto& tilesheets : tileMapJson["tilesets"])
    {

    }



    if(physics2DManager != nullptr)
    {

    }
}

}