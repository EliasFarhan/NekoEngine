#include "gl/model.h"
#include "gl/texture.h"

#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <engine/log.h>

#include "io_system.h"
#include "engine/engine.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko::assimp
{
	void Model::Draw(const gl::Shader& shader)
	{
		for (auto& mesh : meshes_)
			mesh.Draw(shader);
	}

	void Model::Destroy()
	{
		for (auto& mesh : meshes_)
			mesh.Destroy();
		meshes_.clear();
		processModelJob_.Reset();
	}

	Model::Model() : processModelJob_([this]
	{
		ProcessModel();
	})
	{
	}

	void Model::LoadModel(std::string_view path)
	{
		path_ = path;
		directory_ = path.substr(0, path.find_last_of('/'));
		logDebug("ASSIMP: Loading model: " + path_);
		BasicEngine::GetInstance()->ScheduleJob(&processModelJob_, JobThreadType::OTHER_THREAD);
	}

	bool Model::IsLoaded() const
	{
		
		if(!processModelJob_.IsDone())
		{
			return false;
		}
		for(auto& mesh : meshes_)
		{
			if (!mesh.IsLoaded())
				return false;
		}
		
		return true;
	}

	void Model::ProcessModel()
	{
#ifdef EASY_PROFILE_USE
		EASY_BLOCK("Process 3d Model");
		EASY_BLOCK("Load 3d Model");
#endif
		Assimp::Importer import;
		const aiScene* scene = nullptr;
		Job loadingModelJob = Job([this, &import, &scene]
		{
#ifdef EASY_PROFILE_USE
		EASY_BLOCK("Model Disk Load");
#endif
				//assimp delete automatically the IO System
			NekoIOSystem* ioSystem = new NekoIOSystem();
			import.SetIOHandler(ioSystem);

			scene = import.ReadFile(path_.data(),
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		});
		BasicEngine::GetInstance()->ScheduleJob(&loadingModelJob, JobThreadType::RESOURCE_THREAD);
		loadingModelJob.Join();
#ifdef EASY_PROFILE_USE
		EASY_END_BLOCK;
#endif
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::ostringstream oss;
			oss << "[ERROR] ASSIMP " << import.GetErrorString();
			logDebug(oss.str());
			return;
		}
		meshes_.reserve(scene->mNumMeshes);
#ifdef EASY_PROFILE_USE
		EASY_BLOCK("Process Nodes");
#endif
		ProcessNode(scene->mRootNode, scene);
#ifdef EASY_PROFILE_USE
		EASY_END_BLOCK;
#endif
#ifdef EASY_PROFILE_USE
		EASY_BLOCK("Schedule Mesh Job");
#endif
		for(auto& mesh : meshes_)
		{
			mesh.Init();
		}
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			meshes_.emplace_back();
			auto& mesh = meshes_.back();

			aiMesh* assMesh = scene->mMeshes[node->mMeshes[i]];
			mesh.ProcessMesh(assMesh, scene, directory_);
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

}
