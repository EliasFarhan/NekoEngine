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

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif
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
				nodeVariables.vecVariables.emplace_back(element.key(),
                                                        (element.value().get<bool>()) ? "true" : "false");
			}
			if (element.value().is_number())
			{
				nodeVariables.vecVariables.emplace_back(element.key(),
                                                        std::to_string(element.value().get<double>()));
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
					for (const auto& elementArray : element.value().items()) {
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

	std::unique_ptr<BehaviorTreeNode>
		BehaviorTreeManager::ParseJsonCompositeSequence(
			Index comp,
			const json& jsonContent) const
	{
		
		auto nodeVariables =
			ParseJsonVariablesNodes(
				comp, 
				jsonContent, 
				BehaviorTreeElementType::BTT_COMPOSITE);
		auto compositeSequence =
			std::make_unique<BehaviorTreeCompositeSequence>(
				std::move(nodeVariables.vecNodes),
				nodeVariables.vecVariables);
		return compositeSequence;
	}

	std::unique_ptr<BehaviorTreeNode>
		BehaviorTreeManager::ParseJsonCompositeSelector(
			Index comp,
			const json& jsonContent) const
	{
		
		auto nodeVariables =
			ParseJsonVariablesNodes(
				comp, 
				jsonContent, 
				BehaviorTreeElementType::BTT_COMPOSITE);
		auto compositeSelector =
			std::make_unique<BehaviorTreeCompositeSelector>(
				std::move(nodeVariables.vecNodes),
				nodeVariables.vecVariables);
		return compositeSelector;
	}

	std::unique_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonComposite(
		Index comp,
		const json& jsonContent) const
	{
		
		logDebug("WARNING trying to parse a composite node!");
		auto nodeVariables =
			ParseJsonVariablesNodes(
				comp, 
				jsonContent, 
				BehaviorTreeElementType::BTT_COMPOSITE);
		auto composite =
			std::make_unique<BehaviorTreeComposite>(
				std::move(nodeVariables.vecNodes),
				nodeVariables.vecVariables);
		return composite;
	}

	std::unique_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonDecorator(
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
		auto decorator =
			std::make_unique<BehaviorTreeDecorator>(
				comp,
				std::move(nodeVariables.vecNodes[0]),
				nodeVariables.vecVariables);
		assert(nodeVariables.vecNodes.size() == 1);
		//decorator->SetChild(nodeVariables.vecNodes[0]);
		return decorator;
	}

	std::unique_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonLeafCondition(
		Index comp,
		const json& jsonContent) const
	{
		
		auto nodeVariables = ParseJsonVariablesNodes(comp, jsonContent);
		auto condition =
			std::make_unique<BehaviorTreeLeafCondition>(
				comp,
				nodeVariables.vecVariables);
		return condition;
	}

	std::unique_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonLeafWait(
		Index comp,
		const json& jsonContent) const
	{
		
		auto nodeVariables = ParseJsonVariablesNodes(comp, jsonContent);
		auto wait =
			std::make_unique<BehaviorTreeLeafWait>(
				nodeVariables.vecVariables);
		return wait;
	}

	std::unique_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonLeafMoveTo(
		Index comp,
		const json& jsonContent) const
	{
		
		auto nodeVariables = ParseJsonVariablesNodes(comp, jsonContent);
		auto moveTo =
			std::make_unique<BehaviorTreeLeafMoveTo>(
				comp,
				nodeVariables.vecVariables);
		return moveTo;
	}

	std::unique_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonLeafFunctional(
		Index comp,
		const json& jsonContent) const 
	{
		
		auto nodeVariables = ParseJsonVariablesNodes(comp, jsonContent);
		auto functional =
			std::make_unique<BehaviorTreeLeafFunctional>(
				comp,
				nodeVariables.vecVariables);
		return functional;
	}

	std::unique_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonLeaf(
		Index comp,
		const json& jsonContent) const
	{
		
		logDebug("WARNING trying to parse a leaf node!");
		auto nodeVariables = ParseJsonVariablesNodes(comp, jsonContent);
		auto leaf =
			std::make_unique<BehaviorTreeLeaf>(
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

	std::unique_ptr<BehaviorTreeNode> BehaviorTreeManager::ParseJsonObject(
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
		const BehaviorTreeNode* behaviorTree)
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
				static_cast<const BehaviorTreeComposite*>(behaviorTree);
			if (!interfaceComposite)
			{
				logDebug(oss_indent.str() + "ERROR in composite!");
			}
			else
			{
				for (const auto& child : interfaceComposite->GetChildrenList())
				{
					LogBehaviorTree(child.get());
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
			    static_cast<const BehaviorTreeDecorator*>(
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

	std::unique_ptr<BehaviorTreeNode>
		BehaviorTreeManager::ParseBehaviorTreeFromJson(
			Index comp,
			const json& jsonContent) const
	{
		return ParseJsonObject(comp, jsonContent);
	}

	std::unique_ptr<BehaviorTreeNode>
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
		if (futureSize <= comp)
		{
			while (futureSize <= comp)
			{
				futureSize *= 2;
			}
			vecBehaviorTree_.resize(futureSize);
		}
		if (behaviorTree) {
			vecBehaviorTree_.at(comp) = std::move(behaviorTree);
			return comp;
		}
		logDebug("ERROR could not parse the file: {}" + jsonContent.get<std::string>());
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
			vecBehaviorTree_.at(comp) = std::move(behaviorTree);
			return comp;
		}
		logDebug("ERROR could not parse the file: " + jsonFile);
		return INDEX_INVALID;
	}

	void BehaviorTreeManager::LogBehaviorTreeIndex(Index id) const
	{
		assert(id < vecBehaviorTree_.size());
		LogBehaviorTree(vecBehaviorTree_[id].get());
	}

	BehaviorTreeNode*
		BehaviorTreeManager::GetBehaviorTreeRootIndex(Index id) const
    {
		assert(id < vecBehaviorTree_.size());
		return vecBehaviorTree_[id].get();
	}

	BehaviorTreeFlow BehaviorTreeManager::ExecuteIndex(Index id) const
    {
		assert(id < vecBehaviorTree_.size());
		if (vecBehaviorTree_[id] == nullptr)
			return BehaviorTreeFlow::FAILURE;
		return vecBehaviorTree_[id]->Execute();
	}

} // end namespace neko