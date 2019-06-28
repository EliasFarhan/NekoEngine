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
#include "city_function_map.h"
#include "city_behavior_tree.h"

namespace neko {

	class BehaviorTreeManager {
	public:
		// These will probably be removed or turned private.
		void LogBehaviorTree(
			const std::shared_ptr<BehaviorTreeNode> behaviorTree) const;
		std::shared_ptr<BehaviorTreeNode> ParseBehaviorTreeFromJson(
			Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> LoadBehaviorTreeFromJsonFile(
			Index comp,
			const std::string& jsonFile) const;

	public:
		// Index based functions
		Index ParseBehaviorTreeFromJsonIndex(
			Index comp,
			const json& jsonContent);
		Index LoadBehaviorTreeFromJsonFileIndex(
			Index comp,
			const std::string& jsonFile);
		void LogBehaviorTreeIndex(Index id) const;
		std::shared_ptr<BehaviorTreeNode> GetBehaviorTreeRootIndex(Index id);
		BehaviorTreeFlow ExecuteIndex(Index id);

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
			Index comp,
			const json& jsonContent) const;
		NodeVariableDesc ParseJsonVariablesNodes(
			Index comp,
			const json& jsonContent,
			BehaviorTreeElementType behaviorTreeElementType = 
				BehaviorTreeElementType::BTT_LEAF) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonCompositeSequence(
			Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonCompositeSelector(
			Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonComposite(
			Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonDecorator(
			Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonLeafCondition(
			Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonLeafWait(
			Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonLeafMoveTo(
			Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonLeafFunctional(
			Index comp,
			const json& jsonContent) const;
		std::shared_ptr<BehaviorTreeNode> ParseJsonLeaf(
			Index comp,
			const json& jsonContent) const;
		BehaviorTreeObjectType GetTypeFromJson(const json& jsonContent) const;
	
	private:
		std::vector<std::shared_ptr<BehaviorTreeNode>> vecBehaviorTree_;
	};

} // end namespace neko