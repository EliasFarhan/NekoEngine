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

#include <sstream>
#include <City/city_behavior_tree_manager.h>
#include <engine/log.h>

namespace neko {

	BehaviorTreeManager::NodeVariableDesc
		BehaviorTreeManager::ParseJsonVariablesNodes(
			Index comp,
			const json& jsonContent,
			BehaviorTreeElementType behaviorTreeElementType) const
	{
		NodeVariableDesc nodeVariables;
		for (const auto& element : jsonContent.items())
		{
			if (element.value().is_null())
			{
				logDebug("ERROR in parsing json value: " + element.value().get<std::string>());
				return {};
			}
			if (element.value().is_boolean())
			{
				nodeVariables.vecVariables.push_back(
					{ element.key(),
					(element.value().get<bool>()) ? "true" : "false" });
			}
			if (element.value().is_number())
			{
				nodeVariables.vecVariables.push_back(
					{ element.key(),
					std::to_string(element.value().get<double>()) });
			}
			if (element.value().is_object())
			{
				if (behaviorTreeElementType == 
					BehaviorTreeElementType::BTT_DECORATOR)
				{
					ParseJsonObject(comp, element.value());
				}
				else
				{
					logDebug("ERROR in parsing json value: " + element.value().get<std::string>());
					return {};
				}
			}
			if (element.value().is_array())
			{
				if (element.key() == "children" &&
					behaviorTreeElementType == 
					BehaviorTreeElementType::BTT_COMPOSITE)
				{
					for (auto elementArray : element.value().items()) {
						nodeVariables.vecNodes.push_back(
							ParseJsonObject(comp, elementArray.value()));
					}
				}
				else
				{
					logDebug("ERROR in parsing json value: " + element.value().get<std::string>());
					return {};
				}
			}
			if (element.value().is_string())
			{
				nodeVariables.vecVariables.push_back(
					{ element.key(),
					std::string(element.value().get<std::string>()) });
			}
		}
		return nodeVariables;
	}

	std::shared_ptr<BehaviorTreeNode>
		BehaviorTreeManager::ParseJsonCompositeSequence(
			Index comp,
			const json& jsonContent) const
	{
		auto nodeVariables =
			ParseJsonVariablesNodes(
				comp, 
				jsonContent, 
				BehaviorTreeElementType::BTT_COMPOSITE);
		std::shared_ptr<BehaviorTreeCompositeSequence> compositeSequence =
			std::make_shared<BehaviorTreeCompositeSequence>(
				nodeVariables.vecNodes,
				nodeVariables.vecVariables);
		return compositeSequence;
	}

	std::shared_ptr<BehaviorTreeNode>
		BehaviorTreeManager::ParseJsonCompositeSelector(
			Index comp,
			const json& jsonContent) const
	{
		auto nodeVariables =
			ParseJsonVariablesNodes(
				comp, 
				jsonContent, 
				BehaviorTreeElementType::BTT_COMPOSITE);
		std::shared_ptr<BehaviorTreeCompositeSelector> compositeSelector =
			std::make_shared<BehaviorTreeCompositeSelector>(
				nodeVariables.vecNodes,
				nodeVariables.vecVariables);
		return compositeSelector;
	}

	std::shared_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonComposite(
		Index comp,
		const json& jsonContent) const
	{
		logDebug("WARNING trying to parse a composite node!");
		auto nodeVariables =
			ParseJsonVariablesNodes(
				comp, 
				jsonContent, 
				BehaviorTreeElementType::BTT_COMPOSITE);
		std::shared_ptr<BehaviorTreeComposite> composite =
			std::make_shared<BehaviorTreeComposite>(
				nodeVariables.vecNodes,
				nodeVariables.vecVariables);
		return composite;
	}

	std::shared_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonDecorator(
		Index comp,
		const json& jsonContent) const
	{
		logDebug("WARNING trying to parse a decorator node!");
		auto nodeVariables =
			ParseJsonVariablesNodes(
				comp, 
				jsonContent, 
				BehaviorTreeElementType::BTT_DECORATOR);
		assert(nodeVariables.vecNodes.size() == 1);
		std::shared_ptr<BehaviorTreeDecorator> decorator =
			std::make_shared<BehaviorTreeDecorator>(
				comp,
				nodeVariables.vecNodes[0],
				nodeVariables.vecVariables);
		assert(nodeVariables.vecNodes.size() == 1);
		decorator->SetChild(nodeVariables.vecNodes[0]);
		return decorator;
	}

	std::shared_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonLeafCondition(
		Index comp,
		const json& jsonContent) const
	{
		auto nodeVariables = ParseJsonVariablesNodes(comp, jsonContent);
		std::shared_ptr<BehaviorTreeLeafCondition> condition =
			std::make_shared<BehaviorTreeLeafCondition>(
				comp,
				nodeVariables.vecVariables);
		return condition;
	}

	std::shared_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonLeafWait(
		Index comp,
		const json& jsonContent) const
	{
		auto nodeVariables = ParseJsonVariablesNodes(comp, jsonContent);
		std::shared_ptr<BehaviorTreeLeafWait> wait =
			std::make_shared<BehaviorTreeLeafWait>(
				nodeVariables.vecVariables);
		return wait;
	}

	std::shared_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonLeafMoveTo(
		Index comp,
		const json& jsonContent) const
	{
		auto nodeVariables = ParseJsonVariablesNodes(comp, jsonContent);
		std::shared_ptr<BehaviorTreeLeafMoveTo> moveTo =
			std::make_shared<BehaviorTreeLeafMoveTo>(
				comp,
				nodeVariables.vecVariables);
		return moveTo;
	}

	std::shared_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonLeafFunctional(
		Index comp,
		const json& jsonContent) const 
	{
		auto nodeVariables = ParseJsonVariablesNodes(comp, jsonContent);
		std::shared_ptr<BehaviorTreeLeafFunctional> functional =
			std::make_shared<BehaviorTreeLeafFunctional>(
				comp,
				nodeVariables.vecVariables);
		return functional;
	}

	std::shared_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonLeaf(
		Index comp,
		const json& jsonContent) const
	{
		logDebug("WARNING trying to parse a leaf node!");
		auto nodeVariables = ParseJsonVariablesNodes(comp, jsonContent);
		std::shared_ptr<BehaviorTreeLeaf> leaf =
			std::make_shared<BehaviorTreeLeaf>(
				nodeVariables.vecVariables);
		return leaf;
	}

	BehaviorTreeObjectType BehaviorTreeManager::GetTypeFromJson(
		const json& jsonContent) const
	{
		static const std::map<std::string, BehaviorTreeObjectType> reverseMap =
			[]() ->std::map<std::string, BehaviorTreeObjectType>
		{
			std::map<std::string, BehaviorTreeObjectType> reverse;
			std::for_each(mapCompositeString.begin(), mapCompositeString.end(),
				[&reverse](std::pair<BehaviorTreeObjectType, std::string> pair)
			{
				reverse.insert({ pair.second, pair.first });
			});
			return reverse;
		}();
		assert(reverseMap.size() == mapCompositeString.size());
		auto it = reverseMap.find(jsonContent["type"]);
		if (it == reverseMap.end()) {
			return BehaviorTreeObjectType::ERROR_UNKNOWN;
		}
		return it->second;
	}

	std::shared_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonObject(
		Index comp,
		const json& jsonContent) const
	{
		switch (GetTypeFromJson(jsonContent)) {
		case BehaviorTreeObjectType::COMPOSITE_SEQUENCE:
			return ParseJsonCompositeSequence(comp, jsonContent);
		case BehaviorTreeObjectType::COMPOSITE_SELECTOR:
			return ParseJsonCompositeSelector(comp, jsonContent);
		case BehaviorTreeObjectType::INTERFACE_COMPOSITE:
			return ParseJsonComposite(comp, jsonContent);
		case BehaviorTreeObjectType::INTERFACE_DECORATOR:
			return ParseJsonDecorator(comp, jsonContent);
		case BehaviorTreeObjectType::LEAF_CONDITION:
			return ParseJsonLeafCondition(comp, jsonContent);
		case BehaviorTreeObjectType::LEAF_WAIT:
			return ParseJsonLeafWait(comp, jsonContent);
		case BehaviorTreeObjectType::LEAF_MOVE_TO:
			return ParseJsonLeafMoveTo(comp, jsonContent);
		case BehaviorTreeObjectType::LEAF_FUNCTIONAL:
			return ParseJsonLeafFunctional(comp, jsonContent);
		case BehaviorTreeObjectType::INTERFACE_LEAF:
			return ParseJsonLeaf(comp, jsonContent);
		default:
		{
			std::ostringstream oss;
			oss << "Error parsing json: " << jsonContent;
			logDebug(oss.str());
			return {};
		}
		}
		return {};
	}

	void BehaviorTreeManager::LogBehaviorTree(
		const std::shared_ptr<BehaviorTreeNode> behaviorTree) const
	{
		static int indent = 0;
		std::ostringstream oss_indent;
		for (int i = 0; i < indent; ++i)
		{
			oss_indent << "\t";
		}
		if (!behaviorTree)
		{
			logDebug(oss_indent.str() + "ERROR nullptr as a behaviorTree.");
			return;
		}
		logDebug(oss_indent.str() + mapCompositeString.at(behaviorTree->GetType()));
		const auto& mapVariables = behaviorTree->GetVariables();
		for (const std::pair<std::string, std::string>& pair : mapVariables)
		{
			logDebug(oss_indent.str() + pair.first + "\t\t" + pair.second);
		}
		switch (behaviorTree->GetType())
		{
		case BehaviorTreeObjectType::COMPOSITE_SEQUENCE:
		case BehaviorTreeObjectType::COMPOSITE_SELECTOR:
		case BehaviorTreeObjectType::INTERFACE_COMPOSITE:
		{
			logDebug(oss_indent.str() + "children : {");
			indent++;
			const auto interfaceComposite =
				std::dynamic_pointer_cast<const BehaviorTreeComposite>(behaviorTree);
			if (!interfaceComposite)
			{
				logDebug(oss_indent.str() + "ERROR in composite!");
			}
			else
			{
				for (const auto& child : interfaceComposite->GetChildrenList())
				{
					LogBehaviorTree(child);
				}
			}
			indent--;
			logDebug(oss_indent.str() + "}");
			break;
		}
		case BehaviorTreeObjectType::INTERFACE_DECORATOR:
		{
			logDebug(oss_indent.str() + "decorator : {");
			indent++;
			const auto childDecorator =
				std::dynamic_pointer_cast<const BehaviorTreeDecorator>(
					behaviorTree);
			if (!childDecorator)
			{
				logDebug(oss_indent.str() + "ERROR in decorator!");
			}
			else
			{
				LogBehaviorTree(childDecorator->GetChild());
			}
			indent--;
			logDebug(oss_indent.str() + "}");
			break;
		}
		case BehaviorTreeObjectType::LEAF_CONDITION:
		case BehaviorTreeObjectType::LEAF_WAIT:
		case BehaviorTreeObjectType::LEAF_MOVE_TO:
		case BehaviorTreeObjectType::LEAF_FUNCTIONAL:
		case BehaviorTreeObjectType::INTERFACE_LEAF:
		{
			logDebug(oss_indent.str() + "leaf {}");
			break;
		}
		default:
		{
			logDebug(oss_indent.str() + "ERROR (unknown type?)!");
			break;
		}
		}
	}

	std::shared_ptr<BehaviorTreeNode>
		BehaviorTreeManager::ParseBehaviorTreeFromJson(
			Index comp,
			const json& jsonContent) const
	{
		return ParseJsonObject(comp, jsonContent);
	}

	std::shared_ptr<BehaviorTreeNode>
		BehaviorTreeManager::LoadBehaviorTreeFromJsonFile(
			Index comp,
			const std::string & jsonFile) const
	{
		std::unique_ptr<json> jsonPtr = LoadJson(jsonFile);
		return ParseJsonObject(comp, *jsonPtr);
	}

	BehaviorTreeManager::BehaviorTreeManager()
	{
		vecBehaviorTree_.resize(INIT_ENTITY_NMB);
	}

	Index BehaviorTreeManager::ParseBehaviorTreeFromJsonIndex(
		Index comp,
		const json& jsonContent)
	{
		auto behaviorTree = ParseBehaviorTreeFromJson(comp, jsonContent);
		size_t futureSize = vecBehaviorTree_.capacity();
		while (futureSize <= comp)
		{
			futureSize *= 2;
		}
		vecBehaviorTree_.resize(futureSize);
		if (behaviorTree) {
			vecBehaviorTree_.at(comp) = behaviorTree;
			return comp;
		}
		logDebug("ERROR could not parse the file: " + jsonContent.get<std::string>());
		return INDEX_INVALID;
	}

	Index BehaviorTreeManager::LoadBehaviorTreeFromJsonFileIndex(
		Index comp,
		const std::string& jsonFile)
	{
		size_t futureSize = vecBehaviorTree_.capacity();
		while (futureSize <= comp)
		{
			futureSize *= 2;
		}
		vecBehaviorTree_.resize(futureSize);
		auto behaviorTree =
			LoadBehaviorTreeFromJsonFile(comp, jsonFile);
		if (behaviorTree)
		{
			vecBehaviorTree_.at(comp) = behaviorTree;
			return comp;
		}
		logDebug("ERROR could not parse the file: " + jsonFile);
		return INDEX_INVALID;
	}

	void BehaviorTreeManager::LogBehaviorTreeIndex(Index id) const
	{
		assert(id < vecBehaviorTree_.size());
		LogBehaviorTree(vecBehaviorTree_[id]);
	}

	std::shared_ptr<BehaviorTreeNode>
		BehaviorTreeManager::GetBehaviorTreeRootIndex(Index id)
	{
		assert(id < vecBehaviorTree_.size());
		return vecBehaviorTree_[id];
	}

	BehaviorTreeFlow BehaviorTreeManager::ExecuteIndex(Index id)
	{
		assert(id < vecBehaviorTree_.size());
		return vecBehaviorTree_[id]->Execute();
	}

} // end namespace neko