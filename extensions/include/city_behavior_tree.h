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

#include <vector>
#include <memory>
#include <map>
#include "utilities/time_utility.h"
#include "utilities/json_utility.h"

namespace neko
{

	enum BehaviorTreeFlow
	{
		SUCCESS,
		FAILURE,
		RUNNING,
	};

	// Basic type of element in a behavior tree.
	enum BehaviorTreeElementType {
		BTT_COMPOSITE,
		BTT_DECORATOR,
		BTT_LEAF,
	};

	// Object type enum this will give witch type an object is.
	enum CompositeObjectType
	{
		// An error type return in case you don't find the type.
		ERROR_UNKNOWN,
		// Basic types these are not suppose to be in a behavior tree.
		INTERFACE_COMPOSITE,
		INTERFACE_DECORATOR,
		INTERFACE_LEAF,
		// Composite type these are suppose to be in a behavior tree.
		COMPOSITE_SEQUENCE,
		COMPOSITE_SELECTOR,
		// Leaf type also suppose to be in behavior tree.
		LEAF_CONDITION,
		LEAF_WAIT,
		LEAF_MOVE_TO,
	};

	// Object type list used to parse the behavior tree in json format.
	const static std::map<CompositeObjectType, std::string> mapCompositeString = 
	{
		{INTERFACE_COMPOSITE, "interface_composite"},
		{INTERFACE_DECORATOR, "interface_decorator"},
		{INTERFACE_LEAF,      "interface_leaf"},
		// Composite are connected to more than one sub element derives from
		// Composite.
		{COMPOSITE_SEQUENCE,  "composite_sequence"},
		{COMPOSITE_SELECTOR,  "composite_selector"},
		// Leaf component that derives from leaf node.
		{LEAF_CONDITION,      "leaf_condition"},
		{LEAF_WAIT,           "leaf_wait"},
		{LEAF_MOVE_TO,        "leaf_move_to"},
	};

	// Global interface for a node in a behavior tree.
	class BehaviorTreeNode
	{
	public:
		BehaviorTreeNode() = default;
		BehaviorTreeNode(
			const std::vector<std::pair<std::string, std::string>>& il);

		virtual BehaviorTreeFlow Execute() = 0;
		virtual CompositeObjectType GetType() const = 0;
		void SetVariable(const std::string& variable, const std::string& value);
		const std::string GetVariable(const std::string& name) const;
		const std::map<std::string, std::string>& GetVariables() const 
		{
			return variables_;
		}

	protected:
		std::map<std::string, std::string> variables_;
	};

	// Decorator in a behavior tree.
	class BehaviorTreeDecorator : public BehaviorTreeNode
	{
	public:
		BehaviorTreeDecorator() = default;
		BehaviorTreeDecorator(
			const std::vector<std::pair<std::string, std::string>>& il) :
			BehaviorTreeNode(il) {}

		virtual BehaviorTreeFlow Execute() override { return FAILURE; }
		void SetChild(const std::shared_ptr<BehaviorTreeNode>& child)
		{
			child_ = child;
		}
		const std::shared_ptr<BehaviorTreeNode> GetChild() const
		{
			return child_;
		}
		virtual CompositeObjectType GetType() const
		{
			return INTERFACE_DECORATOR;
		}

	protected:
		std::shared_ptr<BehaviorTreeNode> child_;
	};

	// Composite in a behavior tree.
	class BehaviorTreeComposite : public BehaviorTreeNode
	{
	public:
		BehaviorTreeComposite() = default;
		BehaviorTreeComposite(
			const std::vector<std::shared_ptr<BehaviorTreeNode>>& ilNodes) :
			children_(ilNodes) {}
		BehaviorTreeComposite(
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeNode(ilVariables) {}
		BehaviorTreeComposite(
			const std::vector<std::shared_ptr<BehaviorTreeNode>>& ilNodes,
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeNode(ilVariables),
			children_(ilNodes) {}

		void AddChild(const std::shared_ptr<BehaviorTreeNode>& child)
		{
			children_.push_back(child);
		}
		const std::vector<std::shared_ptr<BehaviorTreeNode>> 
			GetChildrenList() const
		{
			return children_;
		}
		virtual BehaviorTreeFlow Execute() override { return FAILURE; }
		virtual CompositeObjectType GetType() const
		{
			return INTERFACE_COMPOSITE;
		}

	protected:
		std::vector<std::shared_ptr<BehaviorTreeNode>> children_;
		uint32_t currentCount_ = 0;
	};

	// Leaf in a behavior tree.
	class BehaviorTreeLeaf : public BehaviorTreeNode
	{
	public:
		BehaviorTreeLeaf() = default;
		BehaviorTreeLeaf(
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeNode(ilVariables) {}

		virtual BehaviorTreeFlow Execute() override { return FAILURE; }
		virtual CompositeObjectType GetType() const
		{
			return INTERFACE_LEAF;
		}
	};

	// Sequence in a behavior tree.
	class BehaviorTreeComponentSequence : public BehaviorTreeComposite
	{
	public:
		BehaviorTreeComponentSequence() = default;
		BehaviorTreeComponentSequence(
			const std::vector<std::shared_ptr<BehaviorTreeNode>>& ilNodes) :
			BehaviorTreeComposite(ilNodes) {}
		BehaviorTreeComponentSequence(
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeComposite(ilVariables) {}
		BehaviorTreeComponentSequence(
			const std::vector<std::shared_ptr<BehaviorTreeNode>>& ilNodes,
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeComposite(ilNodes, ilVariables) {}

		virtual BehaviorTreeFlow Execute() final;
		virtual CompositeObjectType GetType() const final
		{
			return COMPOSITE_SEQUENCE;
		}
	};

	// Selector in a behavior tree.
	class BehaviorTreeComponentSelector : public BehaviorTreeComposite
	{
	public:
		BehaviorTreeComponentSelector() = default;
		BehaviorTreeComponentSelector(
			const std::vector<std::shared_ptr<BehaviorTreeNode>>& ilNodes) :
			BehaviorTreeComposite(ilNodes) {}
		BehaviorTreeComponentSelector(
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeComposite(ilVariables) {}
		BehaviorTreeComponentSelector(
			const std::vector<std::shared_ptr<BehaviorTreeNode>>& ilNodes,
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeComposite(ilNodes, ilVariables) {}

		virtual BehaviorTreeFlow Execute() final;
		virtual CompositeObjectType GetType() const final
		{
			return COMPOSITE_SELECTOR;
		}
	};

	// Leaf Condition in a behavior tree.
	// The leaf check if a condition dictated by
	// SetVariable("condition", "bla == bli");.
	class BehaviorTreeLeafCondition : public BehaviorTreeLeaf
	{
	public:
		BehaviorTreeLeafCondition() = default;
		BehaviorTreeLeafCondition(
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeLeaf(ilVariables) {}

		virtual BehaviorTreeFlow Execute() final;
		virtual CompositeObjectType GetType() const final
		{
			return LEAF_CONDITION;
		}
	};

	// Leaf Wait in a behavior tree.
	// The delay can be set with the SetVariable("delay", "1.0");.
	class BehaviorTreeLeafWait : public BehaviorTreeLeaf
	{
	public:
		BehaviorTreeLeafWait() = default;
		BehaviorTreeLeafWait(
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeLeaf(ilVariables) {}

		virtual BehaviorTreeFlow Execute() final;
		virtual CompositeObjectType GetType() const final
		{
			return LEAF_WAIT;
		}

	private:
		Timer timer_ = { 0.0, 0.0 };
		bool started_ = false;
	};

	// Leaf Move to in a behavior tree.
	// The location can be set by the SetVariable("to", "10.0, 20");.
	class BehaviorTreeLeafMoveTo : public BehaviorTreeLeaf
	{
	public:
		BehaviorTreeLeafMoveTo() = default;
		BehaviorTreeLeafMoveTo(
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeLeaf(ilVariables) {}

		virtual BehaviorTreeFlow Execute() final;
		virtual CompositeObjectType GetType() const final
		{
			return LEAF_MOVE_TO;
		}
	};

	void PrintBehaviorTree(const BehaviorTreeNode* behaviorTree);
	std::shared_ptr<BehaviorTreeNode> ParseBehaviorTreeFromJson(
		const json& jsonContent);
	std::shared_ptr<BehaviorTreeNode> LoadBehaviorTreeFromJsonFile(
		const std::string& jsonFile);

}    // namespace neko