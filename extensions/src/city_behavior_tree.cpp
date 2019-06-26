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

#include <algorithm>
#include <sstream>
#include <iostream>
#include <city_behavior_tree.h>
#include "engine/engine.h"
#include "engine/log.h"

namespace neko
{
	BehaviorTreeNode::BehaviorTreeNode(
		const std::vector<std::pair<std::string, std::string>>& il)
	{
		std::for_each(il.begin(), il.end(),
			[this](std::pair<std::string, std::string> elem) 
		{
			variables_.insert(elem);
		});
	}

	void BehaviorTreeNode::SetVariable(
		const std::string& variable, 
		const std::string& value)
	{
		variables_.insert({ variable, value });
	}

	const std::string BehaviorTreeNode::GetVariable(
		const std::string& variable) const
	{
		auto it = variables_.find(variable);
		if (it != variables_.end())
		{
			return it->second;
		}
		return "<null>";
	}

	BehaviorTreeFlow BehaviorTreeCompositeSequence::Execute()
	{
		if (currentCount_ >= children_.size()) currentCount_ = 0;
		BehaviorTreeFlow flow = children_[currentCount_]->Execute();
		if (flow == RUNNING) return RUNNING;
		if (flow == FAILURE) return FAILURE;
		currentCount_++;
		return SUCCESS;
	}

	BehaviorTreeFlow BehaviorTreeCompositeSelector::Execute()
	{
		if (currentCount_ >= children_.size()) currentCount_ = 0;
		BehaviorTreeFlow flow = children_[currentCount_]->Execute();
		if (flow == RUNNING) return RUNNING;
		if (flow == SUCCESS) return SUCCESS;
		currentCount_++;
		return FAILURE;
	}

	BehaviorTreeFlow BehaviorTreeLeafCondition::Execute()
	{
		// TODO Run the condition and if not succeeded return FAILURE.
		return FAILURE;
	}

	BehaviorTreeFlow BehaviorTreeLeafWait::Execute()
	{
		if (!started_)
		{
			durationDelay_ = 
				std::chrono::milliseconds(static_cast<long long>(
					std::stod(GetVariable("delay")) * 1000.0));
			triggerTimer_ = std::chrono::system_clock::now();
			started_ = true;
		}
		else
		{
			const auto now = std::chrono::system_clock::now();
			const auto delta = now - triggerTimer_;
			if (delta > durationDelay_)
			{
				started_ = false;
				return SUCCESS;
			}
		}
		return RUNNING;
	}

	BehaviorTreeFlow BehaviorTreeLeafMoveTo::Execute()
	{
		if (to_.x == std::numeric_limits<int>::max() &&
			to_.y == std::numeric_limits<int>::max()) 
		{
			if (GetVariable("to") == "<null>")
			{
				logDebug("Could not find \"to\" in a leaf move to node");
				return FAILURE;
			}
			else
			{
				std::istringstream iss(GetVariable("to"));
				std::vector<double> values;
				double value;
				while (iss >> value) 
				{
					values.push_back(value);
				}
				if (values.size() != 2) 
				{
					logDebug(
						"Size of the leaf move to \"to\" is not equal to 2: " +
						std::to_string(values.size()));
					return FAILURE;
				}
				to_.x = static_cast<int>(values[0]);
				to_.y = static_cast<int>(values[1]);
			}
		}
		// TODO move stuff and if not success return RUNNING.
		return SUCCESS;
	}


	struct NodeVariableDesc
	{
		std::vector<std::pair<std::string, std::string>> vecVariables;
		std::vector<std::shared_ptr<BehaviorTreeNode>> vecNodes;
	};

	// Declaration used in the parsing.
	static std::shared_ptr<BehaviorTreeNode> ParseJsonObject(
		const json& jsonContent);

	static NodeVariableDesc ParseJsonVariablesNodes(
		const json& jsonContent,
		BehaviorTreeElementType behaviorTreeElementType = BTT_LEAF)
	{
		NodeVariableDesc nodeVariables;
		for (const auto& element : jsonContent.items())
		{
			if (element.value().is_null())
			{
				logDebug("ERROR in parsing json value: " + element.value());
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
				if (behaviorTreeElementType == BTT_DECORATOR) 
				{
					ParseJsonObject(element.value());
				}
				else
				{
					logDebug("ERROR in parsing json value: " + element.value());
					return {};
				}
			}
			if (element.value().is_array())
			{
				if (element.key() == "children" &&
					behaviorTreeElementType == BTT_COMPOSITE)
				{
					for (auto elementArray : element.value().items()) {
						nodeVariables.vecNodes.push_back(
							ParseJsonObject(elementArray.value()));
					}
				}
				else
				{
					logDebug("ERROR in parsing json value: " + element.value());
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

	static std::shared_ptr<BehaviorTreeNode> ParseJsonCompositeSequence(
		const json& jsonContent)
	{
		auto nodeVariables = ParseJsonVariablesNodes(jsonContent, BTT_COMPOSITE);
		std::shared_ptr<BehaviorTreeCompositeSequence> compositeSequence =
			std::make_shared<BehaviorTreeCompositeSequence>(
				nodeVariables.vecNodes,
				nodeVariables.vecVariables);
		return compositeSequence;
	}

	static std::shared_ptr<BehaviorTreeNode> ParseJsonCompositeSelector(
		const json& jsonContent)
	{
		auto nodeVariables = ParseJsonVariablesNodes(jsonContent, BTT_COMPOSITE);
		std::shared_ptr<BehaviorTreeCompositeSelector> compositeSelector =
			std::make_shared<BehaviorTreeCompositeSelector>(
				nodeVariables.vecNodes,
				nodeVariables.vecVariables);
		return compositeSelector;
	}

	static std::shared_ptr<BehaviorTreeNode> ParseJsonComposite(
		const json& jsonContent) 
	{
		logDebug("WARNING trying to parse a composite node!");
		auto nodeVariables = ParseJsonVariablesNodes(jsonContent, BTT_COMPOSITE);
		std::shared_ptr<BehaviorTreeComposite> composite =
			std::make_shared<BehaviorTreeComposite>(
				nodeVariables.vecNodes,
				nodeVariables.vecVariables);
		return composite;
	}

	static std::shared_ptr<BehaviorTreeNode> ParseJsonDecorator(const json& jsonContent) 
	{
		logDebug("WARNING trying to parse a decorator node!");
		auto nodeVariables = ParseJsonVariablesNodes(jsonContent, BTT_DECORATOR);
		std::shared_ptr<BehaviorTreeDecorator> decorator =
			std::make_shared<BehaviorTreeDecorator>(
				nodeVariables.vecVariables);
		assert(nodeVariables.vecNodes.size() == 1);
		decorator->SetChild(nodeVariables.vecNodes[0]);
		return decorator;
	}

	static std::shared_ptr<BehaviorTreeNode> ParseJsonLeafCondition(const json& jsonContent)
	{
		auto nodeVariables = ParseJsonVariablesNodes(jsonContent);
		std::shared_ptr<BehaviorTreeLeafCondition> condition =
			std::make_shared<BehaviorTreeLeafCondition>(
				nodeVariables.vecVariables);
		return condition;
	}

	static std::shared_ptr<BehaviorTreeNode> ParseJsonLeafWait(const json& jsonContent) 
	{
		auto nodeVariables = ParseJsonVariablesNodes(jsonContent);
		std::shared_ptr<BehaviorTreeLeafWait> wait =
			std::make_shared<BehaviorTreeLeafWait>(
				nodeVariables.vecVariables);
		return wait;
	}

	static std::shared_ptr<BehaviorTreeNode> ParseJsonLeafMoveTo(
		const json& jsonContent) 
	{
		auto nodeVariables = ParseJsonVariablesNodes(jsonContent);
		std::shared_ptr<BehaviorTreeLeafMoveTo> moveTo =
			std::make_shared<BehaviorTreeLeafMoveTo>(
				nodeVariables.vecVariables);
		return moveTo;
	}

	static std::shared_ptr<BehaviorTreeNode> ParseJsonLeaf(
		const json& jsonContent) 
	{
		logDebug("WARNING trying to parse a leaf node!");
		auto nodeVariables = ParseJsonVariablesNodes(jsonContent);
		std::shared_ptr<BehaviorTreeLeaf> leaf =
			std::make_shared<BehaviorTreeLeaf>(
				nodeVariables.vecVariables);
		return leaf;
	}

	static CompositeObjectType GetTypeFromJson(const json& jsonContent) 
	{
		static const std::map<std::string, CompositeObjectType> reverseMap =
			[]() ->std::map<std::string, CompositeObjectType>
		{
			std::map<std::string, CompositeObjectType> reverse;
			std::for_each(mapCompositeString.begin(), mapCompositeString.end(),
				[&reverse](std::pair<CompositeObjectType, std::string> pair)
			{
				reverse.insert({ pair.second, pair.first });
			});
			return reverse;
		}();
		assert(reverseMap.size() == mapCompositeString.size());
		auto it = reverseMap.find(jsonContent["type"]);
		if (it == reverseMap.end()) {
			return ERROR_UNKNOWN;
		}
		return it->second;
	}

	static std::shared_ptr<BehaviorTreeNode> ParseJsonObject(
		const json& jsonContent)
	{
		switch (GetTypeFromJson(jsonContent)) {
		case COMPOSITE_SEQUENCE:
			return ParseJsonCompositeSequence(jsonContent);
		case COMPOSITE_SELECTOR:
			return ParseJsonCompositeSelector(jsonContent);
		case INTERFACE_COMPOSITE:
			return ParseJsonComposite(jsonContent);
		case INTERFACE_DECORATOR:
			return ParseJsonDecorator(jsonContent);
		case LEAF_CONDITION:
			return ParseJsonLeafCondition(jsonContent);
		case LEAF_WAIT:
			return ParseJsonLeafWait(jsonContent);
		case LEAF_MOVE_TO:
			return ParseJsonLeafMoveTo(jsonContent);
		case INTERFACE_LEAF:
			return ParseJsonLeaf(jsonContent);
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

	void LogBehaviorTree(const std::shared_ptr<BehaviorTreeNode> behaviorTree)
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
		case COMPOSITE_SEQUENCE:
		case COMPOSITE_SELECTOR:
		case INTERFACE_COMPOSITE:
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
		case INTERFACE_DECORATOR:
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
		case LEAF_CONDITION:
		case LEAF_WAIT:
		case LEAF_MOVE_TO:
		case INTERFACE_LEAF:
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

	std::shared_ptr<BehaviorTreeNode> ParseBehaviorTreeFromJson(
		const json& jsonContent) 
	{
		return ParseJsonObject(jsonContent);
	}

	std::shared_ptr<BehaviorTreeNode> LoadBehaviorTreeFromJsonFile(
		const std::string & jsonFile)
	{
		std::unique_ptr<json> jsonPtr = LoadJson(jsonFile);
		return ParseJsonObject(*jsonPtr);
	}

} // end namespace neko