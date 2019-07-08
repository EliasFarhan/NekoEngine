/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <graphics/tilemap.h>
#include <utilities/json_utility.h>
#include <utilities/file_utility.h>
#include <graphics/texture.h>
#include <engine/engine.h>
#include <physics/physics.h>
#include <Remotery.h>

namespace neko
{

void TiledMap::Init(const std::string& tilemapPath, TextureManager& textureManager, Physics2dManager* physics2DManager)
{

    const std::string sourceFolderPath = GetFileParentPath(tilemapPath);
    const std::unique_ptr<json> tileMapJsonPtr = LoadJson(tilemapPath);
    const json tileMapJson = *tileMapJsonPtr;

    //TODO import the texture and fill the double buffer vertex arrays of tilesheets
    for (auto& tilesheetPathJson : tileMapJson["tilesets"])
    {
        std::string source = tilesheetPathJson["source"];
        auto tiledSheetJsonPtr = LoadJson(LinkFolderAndFile(sourceFolderPath, source));
        auto tiledSheetJson = *tiledSheetJsonPtr;

        Tiledsheet tileSheet;

        std::string textureSource = tiledSheetJson["image"];
		Index textureIndex = textureManager.LoadTexture(LinkFolderAndFile(sourceFolderPath, textureSource));
		tileSheet.texture = textureManager.GetTexture(textureIndex);
        tileSheet.tileSize = sf::Vector2u(tiledSheetJson["tilewidth"], tiledSheetJson["tileheight"]);
        tileSheet.firstId = tilesheetPathJson["firstgid"];
        tileSheet.size = sf::Vector2u(tileSheet.texture->getSize().x / tileSheet.tileSize.x,
                                      tileSheet.texture->getSize().y / tileSheet.tileSize.y);
        tileSheet.tileNmb = tileSheet.size.x * tileSheet.size.y;

        tileSheet.tilemap[0].setPrimitiveType(sf::Triangles);
        tileSheet.tilemap[1].setPrimitiveType(sf::Triangles);
        tileSheets_.push_back(tileSheet);
    }

    for (auto& layer : tileMapJson["layers"])
    {
        if (layer["type"] == "tilelayer")
        {
            sf::Vector2u layerSize = sf::Vector2u(layer["width"], layer["height"]);
            sf::Vector2u offset = sf::Vector2u(layer["x"], layer["y"]);
            for (auto i = 0u; i < layerSize.x * layerSize.y; i++)
            {
                unsigned x = (i % layerSize.x);
                unsigned y = (i / layerSize.x);

                unsigned tile = layer["data"][i];
                Tiledsheet* currentTilesheet = nullptr;
                for (auto& tilesheet : tileSheets_)
                {
                    if (tile >= tilesheet.firstId && tile < tilesheet.firstId + tilesheet.tileNmb)
                    {
                        currentTilesheet = &tilesheet;
                        break;
                    }
                }
                if (currentTilesheet != nullptr)
                {
	                const auto tileSize = sf::Vector2f(currentTilesheet->tileSize);
                    sf::Vector2f tilesheetPos = sf::Vector2f(
                            ((tile - currentTilesheet->firstId) % currentTilesheet->size.x) * tileSize.x,
                            ((tile - currentTilesheet->firstId) / currentTilesheet->size.x) * tileSize.y);
                    sf::Vertex quad[6];
                    sf::Vector2f topLeft = sf::Vector2f((x + offset.x) * tileSize.x,
                                                        (y + offset.y) * tileSize.y);
                    //Counterclockwise two triangles for a quaq
                    quad[0].position = topLeft;
                    quad[1].position = topLeft + sf::Vector2f(0, tileSize.y);
                    quad[2].position = topLeft + tileSize;
                    quad[3].position = topLeft;
                    quad[4].position = topLeft + tileSize;
                    quad[5].position = topLeft + sf::Vector2f(tileSize.x, 0);

                    quad[0].texCoords = tilesheetPos;
                    quad[1].texCoords = tilesheetPos + sf::Vector2f(0, tileSize.y);
                    quad[2].texCoords = tilesheetPos + tileSize;
                    quad[3].texCoords = tilesheetPos;
                    quad[4].texCoords = tilesheetPos + tileSize;
                    quad[5].texCoords = tilesheetPos + sf::Vector2f(tileSize.x, 0);
                    for (auto& v : quad)
                    {
                        currentTilesheet->tilemap[0].append(v);
                        currentTilesheet->tilemap[1].append(v);
                    }
                }

            }
        }
            //TODO Physics boxes
        else if (layer["type"] == "objectgroup")
        {
            if (physics2DManager != nullptr)
            {
                for(auto& objectJson : layer["objects"])
                {
                    sf::Vector2f position = sf::Vector2f(objectJson["x"], objectJson["y"]);
                    sf::Vector2f size = sf::Vector2f(objectJson["width"], objectJson["height"]);

                    b2BodyDef bodyDef;
                    bodyDef.position = pixel2meter(position+size/2.0f);
                    bodyDef.type = b2_staticBody;

                    b2PolygonShape platformBox;
                    platformBox.SetAsBox(pixel2meter(size.x/2.0f), pixel2meter(size.y/2.0f));

                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &platformBox;
                    neko::Collider boxCollider;
                    boxCollider.entity =2;
                    physics2DManager->colliders_.push_back(boxCollider);
                    physics2DManager->CreateBody(bodyDef, &fixtureDef, 1);

                }
            }
        }
    }


}

void TiledMap::PushCommand(GraphicsManager* graphicsManager)
{
    rmt_ScopedCPUSample(PushTilemapCommands, 0)
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto& tilesheet: tileSheets_)
    {
		// TODO probably move the graphics manager to handle share_ptr also.
        graphicsManager->Draw(&tilesheet.tilemap[frameIndex], tilesheet.texture.get());
    }
}
}