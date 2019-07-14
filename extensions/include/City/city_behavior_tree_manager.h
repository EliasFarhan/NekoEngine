#pragma once
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

#include <memory>
#include <vector>
#include "City/city_function_map.h"
#include "City/city_behavior_tree.h"

namespace city {

	class BehaviorTreeManager 
	{
	public:
		// These will probably be removed or turned private.
		void LogBehaviorTree(
			const std::shared_ptr<BehaviorTreeNode> behaviorTree) const;
		std::shared_ptr<BehaviorTreeNode> ParseBehaviorTreeFromJson(
			neko::Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> LoadBehaviorTreeFromJsonFile(
                neko::Index comp,
			const std::string& jsonFile) const;

	public:
		BehaviorTreeManager();
		// Index based functions
        neko::Index ParseBehaviorTreeFromJsonIndex(
                neko::Index comp,
			const json& jsonContent);
        neko::Index LoadBehaviorTreeFromJsonFileIndex(
                neko::Index comp,
			const std::string& jsonFile);
		void LogBehaviorTreeIndex(neko::Index id) const;
		std::shared_ptr<BehaviorTreeNode> GetBehaviorTreeRootIndex(neko::Index id);
		BehaviorTreeFlow ExecuteIndex(neko::Index id);

	protected:
		struct NodeVariableDesc
		{
			std::vector<std::pair<std::string, std::string>> vecVariables;
			std::vector<std::shared_ptr<BehaviorTreeNode>> vecNodes;
		};
		// Basic type of element in a behavior tree.
		enum class BehaviorTreeElementType : uint32_t {
			BTT_ERROR = 0,
			BTT_COMPOSITE = 1,
			BTT_DECORATOR = 2,
			BTT_LEAF = 3,
		};
		std::shared_ptr<BehaviorTreeNode> ParseJsonObject(
                neko::Index comp,
			const json& jsonContent) const;
		NodeVariableDesc ParseJsonVariablesNodes(
                neko::Index comp,
			const json& jsonContent,
			BehaviorTreeElementType behaviorTreeElementType = 
				BehaviorTreeElementType::BTT_LEAF) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonCompositeSequence(
                neko::Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonCompositeSelector(
                neko::Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonComposite(
                neko::Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonDecorator(
                neko::Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonLeafCondition(
                neko::Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonLeafWait(
                neko::Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonLeafMoveTo(
                neko::Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonLeafFunctional(
                neko::Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonLeaf(
                neko::Index comp,
			const json& jsonContent) const;
		BehaviorTreeObjectType GetTypeFromJson(const json& jsonContent) const;
	
	private:
		std::vector<std::shared_ptr<BehaviorTreeNode>> vecBehaviorTree_;
	};

} // end namespace neko