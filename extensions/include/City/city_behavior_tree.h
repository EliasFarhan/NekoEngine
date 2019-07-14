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
#include <limits>
#include <chrono>
#include "City/city_function_map.h"
#include "utilities/time_utility.h"
#include "utilities/json_utility.h"

namespace city {

	enum class BehaviorTreeFlow : uint32_t
	{
		SUCCESS = 0,
		FAILURE = 1,
		RUNNING = 2,
	};

	// Object type enum this will give witch type an object is.
	enum class BehaviorTreeObjectType : uint32_t
	{
		// An error type return in case you don't find the type.
		ERROR_UNKNOWN = 0,
		// Basic types these are not suppose to be in a behavior tree.
		INTERFACE_COMPOSITE = 1,
		INTERFACE_DECORATOR = 2,
		INTERFACE_LEAF = 3,
		// Composite type these are suppose to be in a behavior tree.
		COMPOSITE_SEQUENCE = 4,
		COMPOSITE_SELECTOR = 5,
		// Leaf type also suppose to be in behavior tree.
		LEAF_CONDITION = 6,
		LEAF_WAIT = 7,
		LEAF_MOVE_TO = 8,
		LEAF_FUNCTIONAL = 9,
	};

	// Object type list used to parse the behavior tree in json format.
	const static std::map<BehaviorTreeObjectType, std::string> mapCompositeString =
	{
		{BehaviorTreeObjectType::INTERFACE_COMPOSITE, "interface_composite"},
		{BehaviorTreeObjectType::INTERFACE_DECORATOR, "interface_decorator"},
		{BehaviorTreeObjectType::INTERFACE_LEAF,      "interface_leaf"},
		// Composite are connected to more than one sub element derives from
		// Composite.
		{BehaviorTreeObjectType::COMPOSITE_SEQUENCE,  "composite_sequence"},
		{BehaviorTreeObjectType::COMPOSITE_SELECTOR,  "composite_selector"},
		// Leaf component that derives from leaf node.
		{BehaviorTreeObjectType::LEAF_CONDITION,      "leaf_condition"},
		{BehaviorTreeObjectType::LEAF_WAIT,           "leaf_wait"},
		{BehaviorTreeObjectType::LEAF_MOVE_TO,        "leaf_move_to"},
		{BehaviorTreeObjectType::LEAF_FUNCTIONAL,     "leaf_functional"},
	};

	// Global interface for a node in a behavior tree.
	class BehaviorTreeNode
	{
	public:
		BehaviorTreeNode() = default;
		BehaviorTreeNode(
			const std::vector<std::pair<std::string, std::string>>& il);

		virtual BehaviorTreeFlow Execute() = 0;
		virtual BehaviorTreeObjectType GetType() const = 0;
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
	// This add a condition to the next tree element it takes a parameter as
	// example: "decorator" : "Inverse()" and will return 
	// Inverse(child.Execute());.
	class BehaviorTreeDecorator : public BehaviorTreeNode
	{
	public:
		BehaviorTreeDecorator(neko::Index comp) : funcMap_(comp) {}
		BehaviorTreeDecorator(
                neko::Index comp,
			std::shared_ptr<BehaviorTreeNode> child) :
			funcMap_(comp),	child_(child) {}
		BehaviorTreeDecorator(
                neko::Index comp,
			const std::vector<std::pair<std::string, std::string>>& il) :
			BehaviorTreeNode(il), funcMap_(comp) {}
		BehaviorTreeDecorator(
                neko::Index comp,
			std::shared_ptr<BehaviorTreeNode> child,
			const std::vector<std::pair<std::string, std::string>>& il) :
			BehaviorTreeNode(il), funcMap_(comp), child_(child) {}

		virtual BehaviorTreeFlow Execute() override;
		void SetChild(const std::shared_ptr<BehaviorTreeNode>& child)
		{
			child_ = child;
		}
		const std::shared_ptr<BehaviorTreeNode> GetChild() const
		{
			return child_;
		}
		virtual BehaviorTreeObjectType GetType() const override
		{
			return BehaviorTreeObjectType::INTERFACE_DECORATOR;
		}

	protected:
		std::string decorator_ = "";
		std::string functionName_ = "";
		FunctionMap funcMap_;
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
		virtual BehaviorTreeFlow Execute() override 
		{ 
			return BehaviorTreeFlow::FAILURE; 
		}
		virtual BehaviorTreeObjectType GetType() const override
		{
			return BehaviorTreeObjectType::INTERFACE_COMPOSITE;
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

		virtual BehaviorTreeFlow Execute() override 
		{ 
			return BehaviorTreeFlow::FAILURE; 
		}
		virtual BehaviorTreeObjectType GetType() const override
		{
			return BehaviorTreeObjectType::INTERFACE_LEAF;
		}
	};

	// Sequence in a behavior tree.
	class BehaviorTreeCompositeSequence : public BehaviorTreeComposite
	{
	public:
		BehaviorTreeCompositeSequence() = default;
		BehaviorTreeCompositeSequence(
			const std::vector<std::shared_ptr<BehaviorTreeNode>>& ilNodes) :
			BehaviorTreeComposite(ilNodes) {}
		BehaviorTreeCompositeSequence(
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeComposite(ilVariables) {}
		BehaviorTreeCompositeSequence(
			const std::vector<std::shared_ptr<BehaviorTreeNode>>& ilNodes,
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeComposite(ilNodes, ilVariables) {}

		virtual BehaviorTreeFlow Execute() final;
		virtual BehaviorTreeObjectType GetType() const final
		{
			return BehaviorTreeObjectType::COMPOSITE_SEQUENCE;
		}
	};

	// Selector in a behavior tree.
	class BehaviorTreeCompositeSelector : public BehaviorTreeComposite
	{
	public:
		BehaviorTreeCompositeSelector() = default;
		BehaviorTreeCompositeSelector(
			const std::vector<std::shared_ptr<BehaviorTreeNode>>& ilNodes) :
			BehaviorTreeComposite(ilNodes) {}
		BehaviorTreeCompositeSelector(
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeComposite(ilVariables) {}
		BehaviorTreeCompositeSelector(
			const std::vector<std::shared_ptr<BehaviorTreeNode>>& ilNodes,
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeComposite(ilNodes, ilVariables) {}

		virtual BehaviorTreeFlow Execute() final;
		virtual BehaviorTreeObjectType GetType() const final
		{
			return BehaviorTreeObjectType::COMPOSITE_SELECTOR;
		}
	};

	// Leaf Condition in a behavior tree.
	// The leaf check if a condition dictated by
	// SetVariable("condition", "IsRunning(0.4)");.
	class BehaviorTreeLeafCondition : public BehaviorTreeLeaf
	{
	public:
		BehaviorTreeLeafCondition(neko::Index comp) : funcMap_(comp) {}
		BehaviorTreeLeafCondition(
                neko::Index comp,
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeLeaf(ilVariables),
			funcMap_(comp) {}

		virtual BehaviorTreeFlow Execute() final;
		virtual BehaviorTreeObjectType GetType() const final
		{
			return BehaviorTreeObjectType::LEAF_CONDITION;
		}

	private:
		FunctionMap funcMap_;
		std::string condition_ = "";
		std::string functionName_ = "";
		std::string valueName_ = "";
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
		virtual BehaviorTreeObjectType GetType() const final
		{
			return BehaviorTreeObjectType::LEAF_WAIT;
		}

	private:
		std::chrono::time_point<std::chrono::system_clock> triggerTimer_;
		std::chrono::milliseconds durationDelay_;
		bool started_ = false;
	};

	// Leaf Move to in a behavior tree.
	// The location can be set by the SetVariable("to", "MoveTo(1, 20)");.
	class BehaviorTreeLeafMoveTo : public BehaviorTreeLeaf
	{
	public:
		BehaviorTreeLeafMoveTo(neko::Index comp) : funcMap_(comp) {}
		BehaviorTreeLeafMoveTo(
                neko::Index comp,
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeLeaf(ilVariables), funcMap_(comp) {}

		virtual BehaviorTreeFlow Execute() final;
		virtual BehaviorTreeObjectType GetType() const final
		{
			return BehaviorTreeObjectType::LEAF_MOVE_TO;
		}

	protected:
		FunctionMap funcMap_;
		std::string functionName_ = "";
		sf::Vector2i to_ = { 
			std::numeric_limits<int>::max() , 
			std::numeric_limits<int>::max() };
	};

	// This is an approach to change all function node by a unique one!
	// example: "functional" : "Function(1.0, 2.0, 3.0)".
	// Functional is suppose to be called once so return only failure or
	// success.
	class BehaviorTreeLeafFunctional : public BehaviorTreeLeaf
	{
	public:
		BehaviorTreeLeafFunctional(neko::Index comp) : funcMap_(comp) {}
		BehaviorTreeLeafFunctional(
                neko::Index comp,
			const std::vector<std::pair<std::string, std::string>>& ilVariables) :
			BehaviorTreeLeaf(ilVariables), funcMap_(comp) {}

		virtual BehaviorTreeFlow Execute() final;
		virtual BehaviorTreeObjectType GetType() const final
		{
			return BehaviorTreeObjectType::LEAF_FUNCTIONAL;
		}

	protected:
		FunctionMap funcMap_;
		std::string functional_ = "";
		std::string functionName_ = "";
		std::vector<double> values_;
	};

}    // namespace neko