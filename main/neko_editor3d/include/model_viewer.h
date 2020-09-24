#pragma once
#include "assimp/Importer.hpp"
#include "engine/system.h"

struct aiScene;

namespace neko::editor
{
class ModelViewer : public SystemInterface, public DrawImGuiInterface
{

public:
  void Init() override;
  void Update(seconds dt) override;
  void Destroy() override;
  void DrawImGui() override;
private:
	void LoadModel(const std::string_view modelPath);
	std::vector<std::string> models_{"None"};
	size_t currentIndex_ = 0;
	Assimp::Importer importer_;
	const aiScene* scene_ = nullptr;
};
}
