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

#include <gtest/gtest.h>
#include <engine/engine.h>
#include <city_graph.h>
#include <city_behavior_tree.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <imgui.h>
#include <Remotery.h>

class BehaviorTreeTest : public ::testing::Test {
protected:
	void SetUp() override {
		jsonBehaviorTree_ = R"(
			{
				"type" : "composite_selector",
				"name" : "Application",
				"children" : [
					{
						"type" : "composite_sequence",
						"name" : "Keep alive",
						"children" : [
							{
								"type" : "leaf_condition",
								"name" : "Energy level is low",
								"condition" : "this.energy < 0.3"
							},
							{
								"type" : "leaf_move_to",
								"name" : "Move to energy source",
								"to" : "energy source"
							},
							{
								"type" : "leaf_wait",
								"name" : "Wait for reload",
								"delay" : "2.0"
							}
						]
					},
					{
						"type" : "composite_sequence",
						"name" : "Patrol",
						"children" : [
							{
								"type" : "leaf_move_to",
								"name" : "Move to A",
								"to" : "a"
							},
							{
								"type" : "leaf_wait",
								"name" : "Wait at point A",
								"delay" : "1.0"
							},
							{
								"type" : "leaf_move_to",
								"name" : "Move to B",
								"to" : "b"
							},								
							{
								"type" : "leaf_wait",
								"name" : "Wait at point B",
								"delay" : "1.0"
							}
						]
					}
				]
			}
		)"_json;
		basicEnginePtr_ = std::make_unique<neko::BasicEngine>();
		basicEnginePtr_->Init();
	}

	void TearDown() override {
		basicEnginePtr_->Destroy();
	}

	json jsonBehaviorTree_;
	std::shared_ptr<neko::BehaviorTreeNode> behaviorTreeNodePtr_;
	std::unique_ptr<neko::BasicEngine> basicEnginePtr_;
};

TEST_F(BehaviorTreeTest, IsInitFromJsonWorking) {
	behaviorTreeNodePtr_ = neko::ParseBehaviorTreeFromJson(jsonBehaviorTree_);
	EXPECT_TRUE(behaviorTreeNodePtr_);
	LogBehaviorTree(behaviorTreeNodePtr_.get());
}