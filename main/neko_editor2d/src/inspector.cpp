#include <tools/inspector.h>
#include <tools/neko_editor.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <utilities/file_utility.h>
#include <engine/log.h>
#include "sfml_engine/texture.h"

namespace neko::editor
{

void Inspector::ShowEntityInfo(neko::Entity entity) const
{
	if (entity == neko::INVALID_ENTITY)
		return;

    //TODO Have own entity name manager
    //ImGui::InputText("Entity Name: ", &sceneManager_.GetCurrentScene().entitiesNames[entity]);
	ImGui::LabelText("Entity Value: ", "%u", entity);
	{
		const auto parentEntity = transformManager_.GetParentEntity(entity);
		if (parentEntity != neko::INVALID_ENTITY)
		{
			ImGui::LabelText("Entity Parent", "%u", parentEntity);
		}
	}
	if (entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::PREFAB)))
	{
		/*
		 *const auto index = sceneManager_.GetComponent(entity);
		if (index != neko::INVALID_INDEX)
		{
			ImGui::LabelText("Prefab Index", "%u", index);
		}
		*/
	}
	if (entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::POSITION2D)))
	{

		bool keepComponent = true;

		if (ImGui::CollapsingHeader("Position2d Component", &keepComponent, ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& pos = positionManager_.GetComponent(entity);
			float posRaw[2] = { pos.x, pos.y };
			if (ImGui::InputFloat2("Position", posRaw))
			{
				positionManager_.SetComponent(entity, Vec2f(posRaw[0], posRaw[1]));
			}
		}


		if (!keepComponent)
		{
			positionManager_.DestroyComponent(entityManager_, entity);
		}
	}

	if (entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::SCALE2D)))
	{

		bool keepComponent = true;
		if (ImGui::CollapsingHeader("Scale2d Component", &keepComponent, ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& scale = scaleManager_.GetComponent(entity);
			float scaleRaw[2] = { scale.x, scale.y };
			if (ImGui::InputFloat2("Scale", scaleRaw))
			{
				scaleManager_.SetComponent(entity, Vec2f(scaleRaw[0], scaleRaw[1]));
			}
		}

		if (!keepComponent)
		{
			scaleManager_.DestroyComponent(entityManager_, entity);
		}
	}

	if (entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::ROTATION2D)))
	{

		bool keepComponent = true;
		if (ImGui::CollapsingHeader("Angle2d Component", &keepComponent, ImGuiTreeNodeFlags_DefaultOpen))
		{
			const auto& angle = rotationManager_.GetComponent(entity);
			float tmpAngle = angle;
			if (ImGui::InputFloat("Angle", &tmpAngle))
			{
				rotationManager_.SetComponent(entity, tmpAngle);
			}
		}

		if (!keepComponent)
		{
			rotationManager_.DestroyComponent(entityManager_, entity);
		}

	}
	if (entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::SPRITE2D)))
	{
		bool keepComponent = true;
		if (ImGui::CollapsingHeader("Sprite2d Component", &keepComponent, ImGuiTreeNodeFlags_DefaultOpen))
		{
			bool dirty = false;
			auto tmpSprite = spriteManager_.GetComponent(entity);

			std::string textureName = "None";
			if(tmpSprite.textureId != sfml::INVALID_TEXTURE_ID)
            {
                textureName = textureManager_.GetTexturePath(tmpSprite.textureId);
            }
			if (ImGui::Button(textureName.c_str()))
			{
				ImGui::OpenPopup("Texture Popup");
			}
			if(ImGui::IsItemClicked(1))
            {
			    editor_.OpenAsset(textureName);
            }
			ImGui::SameLine();
			ImGui::Text("Texture");

			if (ImGui::BeginPopup("Texture Popup"))
			{
				ImGui::Text("Texture Browser");
				ImGui::Separator();
				if (ImGui::Selectable("No Texture"))
				{
					dirty = true;
					tmpSprite.textureId = sfml::INVALID_TEXTURE_ID;
				}
				for (auto& textureNamePair : textureManager_.GetTextureNameMap())
				{
					if (ImGui::Selectable(textureNamePair.second.c_str()))
					{
						dirty = true;
						tmpSprite.textureId = textureNamePair.first;
						//spriteManager_.CopyTexture(tmpSprite.textureId, entity, 1);
						//tmpSprite.sprite.setTexture(textureManager_.GetTexture(textureNamePair.first)->texture);
					}
				}
				ImGui::EndPopup();
			}

			ImGui::PushID("Sprite Layer");
			if(ImGui::InputInt("Layer", &tmpSprite.layer, 1))
			{
				dirty = true;
			}

			ImGui::PopID();
			if(dirty)
			{
				spriteManager_.CopyLayer(tmpSprite.layer, entity, 1);
                if(tmpSprite.textureId != spriteManager_.GetComponent(entity).textureId)
                {
                    spriteManager_.CopyTexture(tmpSprite.textureId, entity, 1);
                }
			}
		}

		if (!keepComponent)
		{
			spriteManager_.DestroyComponent(entityManager_, entity);
		}
	}

	if (entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::SPINE_ANIMATION)))
	{

		auto& spineDrawable = spineManager_.GetComponent(entity);
		auto& spineDrawableInfo = spineManager_.GetInfo(entity);
		bool keepComponent = true;
		if (ImGui::CollapsingHeader("Spine2d Component", &keepComponent, ImGuiTreeNodeFlags_DefaultOpen))
		{
			std::string spineButtonName = spineDrawableInfo.spinePath;
			if (spineButtonName.empty())
			{
				spineButtonName = "None";
			}
			if (ImGui::Button(spineButtonName.c_str()))
			{
				ImGui::OpenPopup("Spine Popup");
			}
			ImGui::SameLine();
			ImGui::Text("Spine");
			ImGui::LabelText("Atlas", "%s", spineDrawableInfo.atlasPath.c_str());
			ImGui::LabelText("Skeleton", "%s", spineDrawableInfo.skeletonDataPath.c_str());

			static std::vector<std::string> spineFileList;
			if (ImGui::BeginPopup("Spine Popup"))
			{
				if (spineFileList.empty())
				{
					neko::IterateDirectory(config_.dataRootPath, [](const std::string_view filename)
						{
							if (filename.find(".spine") != std::string_view::npos)
							{
								spineFileList.emplace_back(filename.data());
							}
						}, true);
				}
				ImGui::Selectable("No Spine File");

				for (auto& spineFilename : spineFileList)
				{
					if (ImGui::Selectable(spineFilename.c_str()))
					{
						auto spineJson = neko::LoadJson(spineFilename);
						spineDrawableInfo.atlasPath = spineJson["atlas"];
						spineDrawableInfo.skeletonDataPath = spineJson["skeleton"];
						spineFileList.clear();
					}

				}
				ImGui::EndPopup();
			}
			else
			{
				spineFileList.clear();
			}
			ImGui::PushID("Spine Layer");
			int layer = spineDrawable.layer;
			if (ImGui::InputInt("Layer", &layer, 1))
			{
				spineManager_.CopyLayer(layer, entity, 1);
			}
			ImGui::PopID();

			ImGui::PushID("Load Spine Component");
			if (ImGui::Button("Load..."))
			{
				if (spineManager_.AddSpineDrawable(entity, spineDrawableInfo.atlasPath,
					spineDrawableInfo.skeletonDataPath))
				{
				    spineManager_.Update(entityManager_, 0.0f);
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
			if (ImGui::Button("Preview..."))
			{
				if (spineDrawable.skeletonDrawable != nullptr)
				{
					editor_.OpenAsset(spineDrawableInfo.spinePath);
				}
			}
			ImGui::PopID();
			if(spineDrawable.skeletonDrawable != nullptr)
            {
			    //Show animation combo
                const auto animations = spineDrawable.skeletonData->animations;
                static int currentAnimIndex = 0;
                if(currentAnimIndex >= spineDrawable.skeletonData->animationsCount)
                {
                    currentAnimIndex = 0;
                }
                const char* currentAnim = spineDrawable.skeletonData->animations[currentAnimIndex]->name;            // Here our selection is a single pointer stored outside the object.
                if (ImGui::BeginCombo("Animation", currentAnim)) // The second parameter is the label previewed before opening the combo.
                {
                    for (int n = 0; n < spineDrawable.skeletonData->animationsCount; n++)
                    {
                        const bool isSelected = (currentAnim == animations[n]->name);
                        if (ImGui::Selectable(animations[n]->name, isSelected))
                        {
                            currentAnimIndex = n;
                            if (spineDrawable.skeletonDrawable != nullptr)
                            {
                                spineManager_.SetAnimationByName(entity, animations[n]->name);
                            }
                        };
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
                    }
                    ImGui::EndCombo();
                }
                //Show skin Name
                auto skins = spineDrawable.skeletonData->skins;
                static int currentSkinIndex = 0;
                if(currentSkinIndex >= spineDrawable.skeletonData->skinsCount)
                {
                    currentSkinIndex = 0;
                }
                const char* currentSkin = spineDrawable.skeletonData->skins[0]->name;            // Here our selection is a single pointer stored outside the object.
                if (ImGui::BeginCombo("Skin", currentSkin)) // The second parameter is the label previewed before opening the combo.
                {
                    for (int n = 0; n < spineDrawable.skeletonData->skinsCount; n++)
                    {
                        const bool isSelected = (currentSkin == skins[n]->name);
                        if (ImGui::Selectable(skins[n]->name, isSelected))
                        {
                            currentSkinIndex = n;
                            if (spineDrawable.skeletonDrawable != nullptr)
                            {
                                spineManager_.SetSkinByName(entity, skins[n]->name);
                            }
                        };
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
                    }
                    ImGui::EndCombo();
                }
            }
		}
		if (!keepComponent)
		{
			spineManager_.DestroyComponent(entityManager_, entity);
		}
		ImGui::Separator();
	}

	if (entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::BODY2D)))
	{
		bool keepComponent = true;
		if (ImGui::CollapsingHeader("Body2d Component", &keepComponent, ImGuiTreeNodeFlags_DefaultOpen))
		{
			bool dirty = false;
			auto tmpBodyDef = bodyDefManager_.GetComponent(entity);
			const char* bodyTypeMap[3] =
			{
					"Static",
					"Kinematic",
					"Dynamic"
			};
			ImGui::Combo("Body Type", reinterpret_cast<int*>(&tmpBodyDef.type), bodyTypeMap, 3);
			dirty = ImGui::InputFloat("Gravity Scale", &tmpBodyDef.gravityScale) or dirty;
			dirty = ImGui::Checkbox("Fixed Rotation", &tmpBodyDef.fixedRotation) or dirty;
			if (!tmpBodyDef.fixedRotation &&
				!entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::ROTATION2D)))
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
			if(dirty)
			{
				bodyDefManager_.SetComponent(entity, tmpBodyDef);
			}
		}

		if (!keepComponent)
		{
			bodyDefManager_.DestroyComponent(entityManager_, entity);
		}
	}
	if (entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::BOX_COLLIDER2D)))
	{
		bool keepComponent = true;
		if (ImGui::CollapsingHeader("Box Collider 2D Component", &keepComponent, ImGuiTreeNodeFlags_DefaultOpen))
		{
			bool dirty = false;
			auto tmpBoxColliderDef = boxColliderDefManager_.GetComponent(entity);

			auto& isSensor = tmpBoxColliderDef.fixtureDef.isSensor;
			dirty = ImGui::Checkbox("Is Sensor", &isSensor) or dirty;

			float size[2] = { tmpBoxColliderDef.size.x, tmpBoxColliderDef.size.y };
			if (ImGui::InputFloat2("Box Size", size))
			{
				dirty = true;
				tmpBoxColliderDef.size = Vec2f(size[0], size[1]);
			}
			float offset[2] = { tmpBoxColliderDef.offset.x, tmpBoxColliderDef.offset.y };
			if (ImGui::InputFloat2("Box Offset", offset))
			{
				dirty = true;
				tmpBoxColliderDef.offset = Vec2f(offset[0], offset[1]);
			}
			if(dirty)
			{
				boxColliderDefManager_.SetComponent(entity, tmpBoxColliderDef);
			}

		}
		if(!keepComponent)
        {
		    boxColliderDefManager_.DestroyComponent(entityManager_, entity);
        }
	}
	if (entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::CIRCLE_COLLIDER2D)))
	{
		bool keepComponent = true;
		if (ImGui::CollapsingHeader("Circle Collider 2D Component", &keepComponent, ImGuiTreeNodeFlags_DefaultOpen))
		{
			bool dirty = false;
			auto tmpCircleColliderDef = circleColliderDefManager_.GetComponent(entity);

			auto& isSensor = tmpCircleColliderDef.fixtureDef.isSensor;
			dirty = ImGui::Checkbox("Is Sensor", &isSensor) or dirty;

			if(ImGui::InputFloat("Circle Radius", &tmpCircleColliderDef.shapeDef.m_radius))
			{
				dirty = true;
			}

			float offset[2] = { tmpCircleColliderDef.shapeDef.m_p.x, tmpCircleColliderDef.shapeDef.m_p.y };
			if (ImGui::InputFloat2("Circle Offset", offset))
			{
				dirty = true;
				tmpCircleColliderDef.shapeDef.m_p = b2Vec2(offset[0], offset[1]);
			}
			if(dirty)
			{
				circleColliderDefManager_.SetComponent(entity, tmpCircleColliderDef);
			}
		}
		if(!keepComponent)
        {
		    circleColliderDefManager_.DestroyComponent(entityManager_, entity);
        }
	}
	/*if (entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::POLYGON_COLLIDER2D)))
	{
		bool keepComponent = true;
		if (ImGui::CollapsingHeader("Polygon Collider 2D Component", &keepComponent, ImGuiTreeNodeFlags_DefaultOpen))
		{

		}
	}*/


	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("Component Popup");
	const static std::map<neko::NekoComponentType, std::string> componentNameMap =
	{
			{neko::NekoComponentType::TRANSFORM2D,       "Transform 2D"},
			{neko::NekoComponentType::POSITION2D,        "Position 2D"},
			{neko::NekoComponentType::SCALE2D,            "Scale 2D"},
			{neko::NekoComponentType::ROTATION2D,         "Angle 2D"},
			{neko::NekoComponentType::SPRITE2D,           "Sprite 2D"},
			{neko::NekoComponentType::BODY2D,             "Body 2D"},
			{neko::NekoComponentType::BOX_COLLIDER2D,     "Box Collider 2D"},
			{neko::NekoComponentType::CIRCLE_COLLIDER2D,  "Circle Collider 2D"},
			//{neko::NekoComponentType::POLYGON_COLLIDER2D, "Polygon Collider 2D"},
			{neko::NekoComponentType::SPINE_ANIMATION,    "Spine Animation 2D"},
	};

	ImGui::SameLine();
	if (ImGui::BeginPopup("Component Popup"))
	{
		ImGui::Text("Component");
		ImGui::Separator();
		for (auto& component : componentNameMap)
		{
			if (entityManager_.HasComponent(entity, neko::EntityMask(component.first)))
				continue;
			if (ImGui::Selectable(component.second.c_str(), false))
			{
				entityManager_.AddComponentType(entity, neko::EntityMask(component.first));
				switch (component.first)
				{
				case neko::NekoComponentType::SCALE2D:
				{
					scaleManager_.AddComponent(entityManager_, entity);
					break;
				}
				case neko::NekoComponentType::POSITION2D:
				{
					positionManager_.AddComponent(entityManager_, entity);
					break;
				}
				case neko::NekoComponentType::ROTATION2D:
				{
					rotationManager_.AddComponent(entityManager_, entity);
					break;
				}
				case neko::NekoComponentType::TRANSFORM2D:
				{

					positionManager_.AddComponent(entityManager_, entity);
					scaleManager_.AddComponent(entityManager_, entity);
					rotationManager_.AddComponent(entityManager_, entity);
					break;
				}
				case neko::NekoComponentType::SPRITE2D:
				{
					spriteManager_.AddComponent(entityManager_, entity);
					break;
				}
				case neko::NekoComponentType::SPINE_ANIMATION:
				{
					spineManager_.AddComponent(entityManager_, entity);
					break;
				}
				case neko::NekoComponentType::BODY2D:
				{

					bodyDefManager_.AddComponent(entityManager_, entity);
					if (!entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::POSITION2D)))
					{
						positionManager_.AddComponent(entityManager_, entity);
					}
					break;
				}
				case neko::NekoComponentType::BOX_COLLIDER2D:
				{
					boxColliderDefManager_.AddComponent(entityManager_, entity);
					break;
				}
				case neko::NekoComponentType::CIRCLE_COLLIDER2D:
				{
					circleColliderDefManager_.AddComponent(entityManager_, entity);
					break;
				}
				case neko::NekoComponentType::POLYGON_COLLIDER2D:
				{
					polygonColliderDefManager_.AddComponent(entityManager_, entity);
					break;
				}
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

Inspector::Inspector(NekoEditorExport& nekoEditorExport) :
	transformManager_(nekoEditorExport.transform2dManager),
	sceneManager_(nekoEditorExport.sceneManager),
	entityManager_(nekoEditorExport.entityManager),
	positionManager_(nekoEditorExport.position2dManager),
	scaleManager_(nekoEditorExport.scale2dManager),
	rotationManager_(nekoEditorExport.rotation2dManager),
	spriteManager_(nekoEditorExport.spriteManager),
	textureManager_(nekoEditorExport.textureManager),
	spineManager_(nekoEditorExport.spineManager),
	bodyDefManager_(nekoEditorExport.bodyDef2dManager),
	boxColliderDefManager_(nekoEditorExport.boxColliderDefManager),
	circleColliderDefManager_(nekoEditorExport.circleColliderDefManager),
	polygonColliderDefManager_(nekoEditorExport.polygonColldierDefManager),
	config_(nekoEditorExport.config),
	editor_(nekoEditorExport.editor)
{
}
}