#include <imgui.h>

#include "io_system.h"
#include "assimp/postprocess.h"
#include "utils/file_utility.h"
#include "engine/engine.h"
#include "gl/model.h"
#include "engine/configuration.h"
#include <model_viewer.h>

namespace neko::editor
{

void ModelViewer::Init()
{
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    IterateDirectory(config.dataRootPath, [this](const std::string_view path)
        {
            const auto extension = GetFilenameExtension(path);
            if (extension == ".fbx" || extension == ".obj" || extension == ".blend")
            {
                models_.emplace_back(path.data());
            }
        }, true);
    auto* ioSystem = new NekoIOSystem(BasicEngine::GetInstance()->GetFilesystem());
    importer_.SetIOHandler(ioSystem);
}

void ModelViewer::Update(seconds dt)
{
}

void ModelViewer::Destroy()
{
}

void ModelViewer::DrawImGui()
{
    ImGui::Begin("Model Viewer");
    if (ImGui::BeginCombo("Model Path", models_[currentIndex_].c_str()))
    {
        for (size_t n = 0; n < models_.size(); n++)
        {
            const bool isSelected = (currentIndex_ == n);
            if (ImGui::Selectable(models_[n].c_str(), isSelected))
            {
                LoadModel(models_[n]);
                currentIndex_ = n;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::End();
    if (scene_ != nullptr)
    {
        ImGui::Begin("Model");
        if (scene_->HasMeshes())
        {
            ImGui::Text("Meshes");
            ImGui::Text("Meshes Nmb: %u", scene_->mNumMeshes);
            for (unsigned i = 0; i < scene_->mNumMeshes; i++)
            {
                auto* mesh = scene_->mMeshes[i];
                if (mesh->HasBones())
                {
                    ImGui::Text("Mesh %s has %u bones", mesh->mName.C_Str(), mesh->mNumBones);
                    for (unsigned j = 0; j < mesh->mNumBones; j++)
                    {
                        auto* bone = mesh->mBones[j];
                        ImGui::Text("Bone %s", bone->mName.C_Str());
                    }
                }
            }
        }
        if (scene_->HasMaterials())
        {
            ImGui::Separator();
            ImGui::Text("Materials");
            ImGui::Text("Materials Nmb: %u", scene_->mNumMaterials);
            for (unsigned i = 0; i < scene_->mNumMaterials; i++)
            {
                const auto* material = scene_->mMaterials[i];
                //List property
                for (unsigned property = 0; property < material->mNumProperties; property++)
                {
                    std::string id = std::string("Material") + std::to_string(i);
                    const auto* materialProperty = material->mProperties[property];
                    id += materialProperty->mKey.C_Str();
                    ImGui::PushID(id.c_str());
                    ImGui::Text("Material Property: %s", materialProperty->mKey.C_Str());
                    ImGui::PopID();
                }
                //List texture in material
                for (int textureType = 0; textureType < aiTextureType_UNKNOWN + 1; textureType++)
                {

                    std::string id = std::string("Material") + std::to_string(i);
                    const unsigned int textureCount = material->GetTextureCount(static_cast<aiTextureType>(textureType));
                    if (textureCount == 0)
                        continue;
                    ImGui::Separator();
                    switch (textureType)
                    {
                    case aiTextureType_AMBIENT:

                        id += "Ambient";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Ambient Nmb: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_AMBIENT_OCCLUSION:

                        id += "AmbientOcclusion";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Ambient Occ Nmb: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_BASE_COLOR:

                        id += "BaseColor";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Base Color Nmb: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_DIFFUSE:

                        id += "Diffuse";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Diffuse Nmb: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_DIFFUSE_ROUGHNESS:

                        id += "DiffuseRoughness";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Diffuse Roughness Nmb: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_DISPLACEMENT:

                        id += "Displacement";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Displacement: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_EMISSION_COLOR:

                        id += "Emission Color";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Emission Color: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_EMISSIVE:

                        id += "Emissive";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Emissive: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_HEIGHT:

                        id += "Height";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Height: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_LIGHTMAP:

                        id += "LightMap";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture LightMap: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_METALNESS:

                        id += "Metalness";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Metalness: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_NORMALS:

                        id += "Normals";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Normals: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_NORMAL_CAMERA:

                        id += "Normal Camera";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Normal Camera: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_NONE:

                        id += "None";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture None: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_OPACITY:

                        id += "Opacity";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Opacity: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_SHININESS:

                        id += "Shininess";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Shininess: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_SPECULAR:

                        id += "Specular";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Specular: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_REFLECTION:

                        id += "Reflection";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Reflection: %u", textureCount);
                        ImGui::PopID();
                        break;
                    case aiTextureType_UNKNOWN:

                        id += "Unknown";
                        ImGui::PushID(id.c_str());
                        ImGui::Text("Texture Unknown: %u", textureCount);
                        ImGui::PopID();
                        break;
                    default:;
                    }
                    for (unsigned textureIndex = 0; textureIndex < textureCount; textureIndex++)
                    {
                        aiString textureName;
                        material->GetTexture(static_cast<aiTextureType>(textureType), textureIndex, &textureName);
                        ImGui::Text("Texture Path: %s", textureName.C_Str());
                    }
                }
            }
        }

        if (scene_->HasTextures())
        {
            ImGui::Separator();
            ImGui::Text("Textures");
            ImGui::Text("Textures Nmb: %u", scene_->mNumTextures);
        }
        if (scene_->HasAnimations())
        {
            ImGui::Separator();
            ImGui::Text("Animations");
            ImGui::Text("Animations Nmb: %u", scene_->mNumAnimations);
            for (unsigned animIndex = 0; animIndex < scene_->mNumAnimations; animIndex++)
            {
                const auto* anim = scene_->mAnimations[animIndex];
                ImGui::Text("Anim Name: %s", anim->mName.C_Str());
            }

        }
        ImGui::End();
    }
}

void ModelViewer::LoadModel(const std::string_view modelPath)
{


    scene_ = importer_.ReadFile(modelPath.data(),
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals |
        aiProcess_CalcTangentSpace);
}
}
