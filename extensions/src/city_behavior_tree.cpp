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

namespace neko {

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
		if (flow == BehaviorTreeFlow::RUNNING) 
			return BehaviorTreeFlow::RUNNING;
		if (flow == BehaviorTreeFlow::FAILURE) 
			return BehaviorTreeFlow::FAILURE;
		currentCount_++;
		return BehaviorTreeFlow::SUCCESS;
	}

	BehaviorTreeFlow BehaviorTreeCompositeSelector::Execute()
	{
		if (currentCount_ >= children_.size()) currentCount_ = 0;
		BehaviorTreeFlow flow = children_[currentCount_]->Execute();
		if (flow == BehaviorTreeFlow::RUNNING) 
			return BehaviorTreeFlow::RUNNING;
		if (flow == BehaviorTreeFlow::SUCCESS) 
			return BehaviorTreeFlow::SUCCESS;
		currentCount_++;
		return BehaviorTreeFlow::FAILURE;
	}

	BehaviorTreeFlow BehaviorTreeDecorator::Execute()
	{
		std::string decorator = GetVariable("decorator");
		if (decorator != decorator_)
		{
			decorator_ = decorator;
			std::stringstream iss(decorator_);
			std::getline(iss, functionName_, '(');
			std::string values;
			std::getline(iss, values, ')');
			if (!values.empty())
			{
				logDebug(
					"ERROR found value in decorator node function name: " +
					functionName_);
			}
		}
		BehaviorTreeFlow behaviorTreeFlow = child_->Execute();
		return (funcMap_.CallFunction(
			functionName_, 
			{ static_cast<double>(behaviorTreeFlow) })) ?
			BehaviorTreeFlow::SUCCESS :
			BehaviorTreeFlow::FAILURE;
	}

	BehaviorTreeFlow BehaviorTreeLeafCondition::Execute()
	{
		std::string condition = GetVariable("condition");
		if (condition != condition_)
		{
			condition_ = condition;
			std::stringstream iss(condition_);
			std::getline(iss, functionName_, '(');
			std::getline(iss, valueName_, ')');
		}
		if (functionName_.empty() || valueName_.empty()) 
		{
			return BehaviorTreeFlow::FAILURE;
		}
		return (funcMap_.CallFunction(functionName_, std::stod(valueName_))) ?
			BehaviorTreeFlow::SUCCESS : 
			BehaviorTreeFlow::FAILURE;
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
				return BehaviorTreeFlow::SUCCESS;
			}
		}
		return BehaviorTreeFlow::RUNNING;
	}

	BehaviorTreeFlow BehaviorTreeLeafMoveTo::Execute()
	{
		if (to_.x == std::numeric_limits<int>::max() &&
			to_.y == std::numeric_limits<int>::max()) 
		{
			if (GetVariable("to") == "<null>")
			{
				logDebug("Could not find \"to\" in a leaf move to node");
				return BehaviorTreeFlow::FAILURE;
			}
			else
			{
				std::string valuesStr;
				{
					std::istringstream iss(GetVariable("to"));
					std::getline(iss, functionName_, '(');
					std::getline(iss, valuesStr, ')');
				}
				{
					std::istringstream iss(valuesStr);
					std::vector<double> values;
					std::string value = "";
					while (std::getline(iss, value, ','))
					{
						if (value.empty()) 
						{
							continue;
						}
						values.push_back(std::stod(value));
					}
					if (values.size() != 2)
					{
						logDebug(
							"Arguments of the leaf move to \"to\" <" +
							functionName_ +
							"> is not equal to 2 but: " +
							std::to_string(values.size()));
						return BehaviorTreeFlow::FAILURE;
					}
					to_.x = static_cast<int>(values[0]);
					to_.y = static_cast<int>(values[1]);
				}
			}
		}
		return (funcMap_.CallFunction(
			functionName_,
			{ static_cast<double>(to_.x), static_cast<double>(to_.y) })) ?
			BehaviorTreeFlow::SUCCESS : 
			BehaviorTreeFlow::FAILURE;
	}

	BehaviorTreeFlow BehaviorTreeLeafFunctional::Execute()
	{
		std::string functional = GetVariable("functional");
		std::vector<double> values;
		if (functional != functional_) 
		{
			functional_ = functional;
			std::string valuesStr;
			{
				std::istringstream iss(functional_);
				std::getline(iss, functionName_, '(');
				std::getline(iss, valuesStr, ')');
			}
			{
				std::istringstream iss(valuesStr);
				std::string value = "";
				while (std::getline(iss, value, ','))
				{
					if (value.empty())
					{
						continue;
					}
					values.push_back(std::stod(value));
				}
			}
		}
		return (funcMap_.CallFunction(functionName_, values)) ?
			BehaviorTreeFlow::SUCCESS :
			BehaviorTreeFlow::FAILURE;
	}

} // end namespace neko