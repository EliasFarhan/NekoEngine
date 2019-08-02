#include <tools/inspector.h>
#include <tools/neko_editor.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <utilities/file_utility.h>
#include <engine/log.h>

namespace editor
{

void Inspector::ShowEntityInfo(neko::Entity entity)
{
    if (entity == neko::INVALID_ENTITY)
        return;

    auto& entityManager = nekoEditor_.GetEntityManager();
    auto& sceneManager = nekoEditor_.GetSceneManager();

    ImGui::InputText("Entity Name: ", &sceneManager.GetCurrentScene().entitiesNames[entity]);
    if (entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::POSITION2D)))
    {

        auto& positionManager = nekoEditor_.GetPositionManager();
        bool keepComponent = true;

        if (ImGui::CollapsingHeader("Position2d Component", &keepComponent))
        {
            auto& pos = positionManager.GetComponent(entity);
            float posRaw[2] = {pos.x, pos.y};
            if (ImGui::InputFloat2("Position", posRaw))
            {
                positionManager.SetComponent(entity, sf::Vector2f(posRaw[0], posRaw[1]));
            }
        }


        if (!keepComponent)
        {
            positionManager.DestroyComponent(entityManager, entity);
        }
    }

    if (entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::SCALE2D)))
    {

        auto& scaleManager = nekoEditor_.GetScaleManager();
        bool keepComponent = true;
        if (ImGui::CollapsingHeader("Scale2d Component", &keepComponent))
        {
            auto& scale = scaleManager.GetComponent(entity);
            float scaleRaw[2] = {scale.x, scale.y};
            if (ImGui::InputFloat2("Scale", scaleRaw))
            {
                scaleManager.SetComponent(entity, sf::Vector2f(scaleRaw[0], scaleRaw[1]));
            }
        }

        if (!keepComponent)
        {
            scaleManager.DestroyComponent(entityManager, entity);
        }
    }

    if (entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::ANGLE2D)))
    {
        auto& angleManager = nekoEditor_.GetAngleManager();
        bool keepComponent = true;
        if (ImGui::CollapsingHeader("Angle2d Component", &keepComponent))
        {
            auto& angle = angleManager.GetComponent(entity);
            if (ImGui::InputFloat("Angle", &angle))
            {
                angleManager.SetComponent(entity, angle);
            }
        }

        if (!keepComponent)
        {
            angleManager.DestroyComponent(entityManager, entity);
        }

    }
    if (entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::SPRITE2D)))
    {

        auto& spriteManager = nekoEditor_.GetSpriteManager();
        bool keepComponent = true;
        if (ImGui::CollapsingHeader("Sprite2d Component", &keepComponent))
        {
            auto& textureManager = nekoEditor_.GetTextureManager();
            auto& sprite = spriteManager.GetComponent(entity);
            const auto& textureName = textureManager.GetTexturePath(sprite.textureId);
            if (ImGui::Button(textureName.c_str()))
            {
                ImGui::OpenPopup("Texture Popup");

            }
            ImGui::SameLine();
            ImGui::Text("Texture");

            if (ImGui::BeginPopup("Texture Popup"))
            {
                ImGui::Text("Texture Browser");
                ImGui::Separator();
                if (ImGui::Selectable("No Texture"))
                {
                    spriteManager.CopyTexture(neko::INVALID_INDEX, entity, 1);
                }
                for (neko::Index i = 0; i < textureManager.GetTextureCount(); i++)
                {
                    if (ImGui::Selectable(textureManager.GetTexturePath(i).c_str()))
                    {
                        spriteManager.CopyTexture(i, entity, 1);
                    }
                }
                ImGui::EndPopup();
            }
            auto& origin = sprite.origin;
            float originRaw[2] = {origin.x, origin.y};
            if (ImGui::InputFloat2("Origin", originRaw))
            {
                origin.x = originRaw[0];
                origin.y = originRaw[1];
            }
            ImGui::InputInt("Layer", &sprite.layer);

        }

        if (!keepComponent)
        {
            spriteManager.DestroyComponent(entityManager, entity);
        }
    }

    if (entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::SPINE_ANIMATION)))
    {
        auto& spineManager = nekoEditor_.GetSpineManager();
        auto& spineDrawable = spineManager.GetComponent(entity);
        auto& spineDrawableInfo = spineManager.GetInfo(entity);
        if (ImGui::Button(spineDrawableInfo.atlasPath.c_str()))
        {
            ImGui::OpenPopup("Atlas Popup");

        }
        ImGui::SameLine();
        ImGui::Text("Atlas");

        static std::vector<std::string> atlasList;
        if (ImGui::BeginPopup("Atlas Popup"))
        {
            if (atlasList.empty())
            {
                neko::IterateDirectory(nekoEditor_.config.dataRootPath, [this](const std::string_view filename)
                {
                    if (filename.find(".atlas.txt") != std::string_view::npos)
                    {
                        atlasList.push_back(filename.data());
                    }
                }, true);
            }
            if (ImGui::Selectable("No Atlas"))
            {
                spineDrawableInfo.atlasPath = "";
            }
            for (auto& atlasFilename : atlasList)
            {
                if (ImGui::Selectable(atlasFilename.c_str()))
                {
                    spineDrawableInfo.atlasPath = atlasFilename;
                }

            }
            ImGui::EndPopup();
        }
        else
        {
            atlasList.clear();
        }

        if (ImGui::Button(spineDrawableInfo.skeletonDataPath.c_str()))
        {
            ImGui::OpenPopup("Skeleton Data Popup");
        }
        ImGui::SameLine();
        ImGui::Text("SkeletonData");

        static std::vector<std::string> skeletonDataList;
        if (ImGui::BeginPopup("Skeleton Data Popup"))
        {
            if (skeletonDataList.empty())
            {
                neko::IterateDirectory(nekoEditor_.config.dataRootPath, [this](const std::string_view filename)
                {
                    if (filename.find(".json") != std::string_view::npos)
                    {
                        skeletonDataList.push_back(filename.data());
                    }
                }, true);
            }
            if (ImGui::Selectable("No Skeleton Data"))
            {
                spineDrawableInfo.skeletonDataPath = "";
            }
            for (auto& skeletonDataFilename : skeletonDataList)
            {
                if (ImGui::Selectable(skeletonDataFilename.c_str()))
                {
                    spineDrawableInfo.skeletonDataPath = skeletonDataFilename;
                }
            }

            ImGui::EndPopup();
        }
        else
        {
            skeletonDataList.clear();
        }
        ImGui::PushID("Load Spine Component");
        if(ImGui::Button("Load..."))
        {
            if(spineManager.AddSpineDrawable(entity, spineDrawableInfo.atlasPath, spineDrawableInfo.skeletonDataPath))
            {
                logDebug("Successfully load spine drawable");
            }
            else
            {
                logDebug("[Error] Could not load spine drawable");
            }
        }
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::PushID("Preview Spine Component");
        if(ImGui::Button("Preview..."))
        {
            if(spineDrawable.skeletonDrawable != nullptr)
            {
                auto& previewer = nekoEditor_.GetPreviewer();
                previewer.SetSpineAnimation(&spineDrawable);
            }
            else
            {
                logDebug("[Error] Load the spine component first");
            }
        }
        ImGui::PopID();
        ImGui::Separator();
    }

    if (entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::BODY2D)))
    {
        auto& bodyDefManager = nekoEditor_.GetBodyDefManager();
        bool keepComponent = true;
        if (ImGui::CollapsingHeader("Body2d Component", &keepComponent))
        {
            auto& bodyDef = bodyDefManager.GetComponent(entity);
            const char* bodyTypeMap[3] =
                    {
                            "Static",
                            "Kinematic",
                            "Dynamic"
                    };
            ImGui::Combo("Body Type", (int*) (&bodyDef.type), bodyTypeMap, 3);
            ImGui::InputFloat("Gravity Scale", &bodyDef.gravityScale);
            ImGui::Checkbox("Fixed Rotation", &bodyDef.fixedRotation);
            if (!bodyDef.fixedRotation &&
                !entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::ANGLE2D)))
            {
                ImGui::SameLine();
                ImGui::TextColored(ImColor(255, 0, 0), "Warning!");
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::TextColored(ImColor(255, 0, 0), "[Warning] Rotation without Angle component");
                    ImGui::EndTooltip();
                }
            }
        }

        if (!keepComponent)
        {
            bodyDefManager.DestroyComponent(entityManager, entity);
        }
    }

    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("Component Popup");
    const static std::map<neko::NekoComponentType, std::string> componentNameMap =
            {
                    {neko::NekoComponentType::POSITION2D,      "Position 2D"},
                    {neko::NekoComponentType::SCALE2D,         "Scale 2D"},
                    {neko::NekoComponentType::ANGLE2D,         "Angle 2D"},
                    {neko::NekoComponentType::SPRITE2D,        "Sprite 2D"},
                    {neko::NekoComponentType::BODY2D,          "Body 2D"},
                    {neko::NekoComponentType::SPINE_ANIMATION, "Spine Animation 2D"},
            };

    ImGui::SameLine();
    if (ImGui::BeginPopup("Component Popup"))
    {
        ImGui::Text("Component");
        ImGui::Separator();
        for (auto& component: componentNameMap)
        {
            if (entityManager.HasComponent(entity, neko::EntityMask(component.first)))
                continue;
            if (ImGui::Selectable(component.second.c_str(), false))
            {
                entityManager.AddComponentType(entity, neko::EntityMask(component.first));
                switch (component.first)
                {
                    case neko::NekoComponentType::SCALE2D:
                    {
                        auto& scaleManager = nekoEditor_.GetScaleManager();
                        scaleManager.AddComponent(entityManager, entity);
                        break;
                    }
                    case neko::NekoComponentType::POSITION2D:
                    {
                        auto& positionManager = nekoEditor_.GetPositionManager();
                        positionManager.AddComponent(entityManager, entity);
                        break;
                    }
                    case neko::NekoComponentType::ANGLE2D:
                    {
                        auto& angleManager = nekoEditor_.GetAngleManager();
                        angleManager.AddComponent(entityManager, entity);
                        break;
                    }
                    case neko::NekoComponentType::SPRITE2D:
                    {
                        auto& spriteManager = nekoEditor_.GetSpriteManager();
                        spriteManager.AddComponent(entityManager, entity);
                        break;
                    }
                    case neko::NekoComponentType::SPINE_ANIMATION:
                    {
                        auto& spineManager = nekoEditor_.GetSpineManager();
                        spineManager.AddComponent(entityManager, entity);
                        break;
                    }
                    case neko::NekoComponentType::BODY2D:
                    {
                        auto& bodyDefManager = nekoEditor_.GetBodyDefManager();
                        bodyDefManager.AddComponent(entityManager, entity);
                        if (!entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::POSITION2D)))
                        {
                            auto& positionManager = nekoEditor_.GetPositionManager();
                            positionManager.AddComponent(entityManager, entity);
                        }
                        break;
                    }
                    case neko::NekoComponentType::COLLIDER2D:
                        break;
                    case neko::NekoComponentType::CONVEX_SHAPE2D:
                        break;
                    default:
                        break;
                }
                ImGui::CloseCurrentPopup();
                break;
            }
        }
        ImGui::EndPopup();
    }
}

void Inspector::BeginWindow()
{
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
}

void Inspector::EndWindow()
{
    ImGui::End();
}
}