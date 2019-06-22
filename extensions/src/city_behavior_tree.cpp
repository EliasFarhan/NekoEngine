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
#include <city_behavior_tree.h>
#include "engine/engine.h"

namespace neko {

	void BehaviorTreeNode::SetVariable(const std::string & variable, const std::string & value)
	{
		variables_.insert({ variable, value });
	}

	std::string BehaviorTreeNode::GetVariable(const std::string & variable) const
	{
		auto it = variables_.find(variable);
		if (it != variables_.end()) {
			return it->second;
		}
		return "null";
	}

	BehaviorTreeFlow BehaviorTreeComponentSequence::Execute()
	{
		if (currentCount_ >= children_.size()) currentCount_ = 0;
		BehaviorTreeFlow flow = children_[currentCount_]->Execute();
		currentCount_++;
		if (flow == RUNNING) return RUNNING;
		if (flow == FAILURE) return FAILURE;
		return SUCCESS;
	}

	BehaviorTreeFlow BehaviorTreeComponentSelector::Execute()
	{
		if (currentCount_ >= children_.size()) currentCount_ = 0;
		BehaviorTreeFlow flow = children_[currentCount_]->Execute();
		currentCount_++;
		if (flow == RUNNING) return RUNNING;
		if (flow == SUCCESS) return SUCCESS;
		return FAILURE;
	}

	BehaviorTreeFlow BehaviorTreeWait::Execute()
	{
		if (!started_) {
			float delay = std::stof(GetVariable("delay"));
			timer_.time = 0.0f;
			timer_.period = delay;
			timer_.Reset();
			started_ = true;
			return RUNNING;
		}
		timer_.Update(MainEngine::GetInstance()->dt.asSeconds());
		if (timer_.IsOver()) {
			started_ = false;
			return SUCCESS;
		}
		return RUNNING;
	}

	BehaviorTreeFlow BehaviorTreeLeafMoveTo::Execute()
	{
		return SUCCESS;
	}

} // end namespace neko