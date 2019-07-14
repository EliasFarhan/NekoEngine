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
#include <engine/log.h>
#include <City/city_graph.h>
#include <City/city_function_map.h>
#include <City/city_behavior_tree.h>
#include <City/city_behavior_tree_manager.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <imgui.h>
#include <Remotery.h>

class BehaviorTreeManagerTest : public ::testing::Test, public neko::BasicEngine 
{
protected:
	void SetUp() override 
	{
		jsonBehaviorTree_ = R"JSON(
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
								"condition" : "EnergyLevel(0.3)"
							},
							{
								"type" : "leaf_move_to",
								"name" : "Move to energy source",
								"to" : "MoveTo(12, 34)"
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
								"to" : "MoveTo(12, 34)"
							},
							{
								"type" : "leaf_wait",
								"name" : "Wait at point A",
								"delay" : "1.0"
							},
							{
								"type" : "leaf_move_to",
								"name" : "Move to B",
								"to" : "MoveTo(12, 34)"
							},								
							{
								"type" : "leaf_wait",
								"name" : "Wait at point B",
								"delay" : "1.0"
							}
						]
					}
				]
			})JSON"_json;
		Init();
	}

	void TearDown() override 
	{
		Destroy();
	}

	json jsonBehaviorTree_;
	city::BehaviorTreeManager behaviorTreeManager_;
};

TEST_F(BehaviorTreeManagerTest, IsInitFromJsonWorking) 
{
	const neko::Index comp = 0xbeef;
	EXPECT_TRUE(behaviorTreeManager_.ParseBehaviorTreeFromJson(
		comp, 
		jsonBehaviorTree_));
}

TEST_F(BehaviorTreeManagerTest, IsBehaviorTreeCorrect) 
{
	const neko::Index comp = 0xbeef;
	auto behaviorTreeNodePtr_ = 
		behaviorTreeManager_.ParseBehaviorTreeFromJson(
			comp, 
			jsonBehaviorTree_);
	EXPECT_TRUE(behaviorTreeNodePtr_);
	{	// Base selector
		std::shared_ptr<city::BehaviorTreeCompositeSelector> selector =
			std::dynamic_pointer_cast<city::BehaviorTreeCompositeSelector>(
				behaviorTreeNodePtr_);
		ASSERT_TRUE(selector);
		const std::vector<std::shared_ptr<city::BehaviorTreeNode>>& 
			selector_children =
			selector->GetChildrenList();
		EXPECT_EQ(selector_children.size(), 2);
		{	// First sequence
			std::shared_ptr<city::BehaviorTreeCompositeSequence> sequence =
				std::dynamic_pointer_cast<city::BehaviorTreeCompositeSequence>(
					selector_children[0]);
			ASSERT_TRUE(sequence);
			const std::vector<std::shared_ptr<city::BehaviorTreeNode>>& 
				sequence_children =
				sequence->GetChildrenList();
			EXPECT_EQ(sequence_children.size(), 3);
			{
				std::shared_ptr<city::BehaviorTreeLeafCondition> condition =
					std::dynamic_pointer_cast<city::BehaviorTreeLeafCondition>(
						sequence_children[0]);
				ASSERT_TRUE(condition);
				EXPECT_EQ(
					condition->GetVariable("condition"), "EnergyLevel(0.3)");
			}
			{
				std::shared_ptr<city::BehaviorTreeLeafMoveTo> move_to =
					std::dynamic_pointer_cast<city::BehaviorTreeLeafMoveTo>(
						sequence_children[1]);
				ASSERT_TRUE(move_to);
				EXPECT_EQ(move_to->GetVariable("to"), "MoveTo(12, 34)");
			}
			{
				std::shared_ptr<city::BehaviorTreeLeafWait> leaf_wait =
					std::dynamic_pointer_cast<city::BehaviorTreeLeafWait>(
						sequence_children[2]);
				ASSERT_TRUE(leaf_wait);
				EXPECT_EQ(leaf_wait->GetVariable("delay"), "2.0");
			}
		}
		{	// Second sequence
			std::shared_ptr<city::BehaviorTreeCompositeSequence> sequence =
				std::dynamic_pointer_cast<city::BehaviorTreeCompositeSequence>(
					selector_children[1]);
			ASSERT_TRUE(sequence);
			const std::vector<std::shared_ptr<city::BehaviorTreeNode>>& 
				sequence_children =
				sequence->GetChildrenList();
			EXPECT_EQ(sequence_children.size(), 4);
			{
				std::shared_ptr<city::BehaviorTreeLeafMoveTo> move_to =
					std::dynamic_pointer_cast<city::BehaviorTreeLeafMoveTo>(
						sequence_children[0]);
				ASSERT_TRUE(move_to);
				EXPECT_EQ(move_to->GetVariable("to"), "MoveTo(12, 34)");
			}
			{
				std::shared_ptr<city::BehaviorTreeLeafWait> leaf_wait =
					std::dynamic_pointer_cast<city::BehaviorTreeLeafWait>(
						sequence_children[1]);
				ASSERT_TRUE(leaf_wait);
				EXPECT_EQ(leaf_wait->GetVariable("delay"), "1.0");
			}
			{
				std::shared_ptr<city::BehaviorTreeLeafMoveTo> move_to =
					std::dynamic_pointer_cast<city::BehaviorTreeLeafMoveTo>(
						sequence_children[2]);
				ASSERT_TRUE(move_to);
				EXPECT_EQ(move_to->GetVariable("to"), "MoveTo(12, 34)");
			}
			{
				std::shared_ptr<city::BehaviorTreeLeafWait> leaf_wait =
					std::dynamic_pointer_cast<city::BehaviorTreeLeafWait>(
						sequence_children[3]);
				ASSERT_TRUE(leaf_wait);
				EXPECT_EQ(leaf_wait->GetVariable("delay"), "1.0");
			}
		}
	}
}

TEST_F(BehaviorTreeManagerTest, IsBehaviorTreeExecuteCorrect) 
{
	const neko::Index comp = 0xbeef;
	neko::Index id = behaviorTreeManager_.ParseBehaviorTreeFromJsonIndex(
			comp,
			jsonBehaviorTree_);
	EXPECT_TRUE(id != neko::INVALID_INDEX);
	city::FunctionMap funcMap(comp);
	funcMap.SetFunction(
		"EnergyLevel", 
		[](neko::Index comp, const std::vector<double>& values)
	{
		EXPECT_EQ(0xbeef, comp);
		EXPECT_EQ(values.size(), 1);
		EXPECT_EQ(values[0], 0.3);
		return false;
	});
	funcMap.SetFunction(
		"MoveTo",
		[](neko::Index comp, const std::vector<double>& values) 
	{
		EXPECT_EQ(values.size(), 2);
		EXPECT_EQ(values[0], 12.0);
		EXPECT_EQ(values[1], 34.0);
		return true;
	});
	// Failure to the condition
	EXPECT_EQ(
		behaviorTreeManager_.ExecuteIndex(id), 
		city::BehaviorTreeFlow::FAILURE);
	// Move to a
	EXPECT_EQ(
		behaviorTreeManager_.ExecuteIndex(id), 
		city::BehaviorTreeFlow::SUCCESS);
	// Wait at a
	city::BehaviorTreeFlow status;
	do 
	{
		status = behaviorTreeManager_.ExecuteIndex(id);
		if (status != city::BehaviorTreeFlow::SUCCESS)
		{
			EXPECT_EQ(status, city::BehaviorTreeFlow::RUNNING);
		}
	} 
	while (status != city::BehaviorTreeFlow::SUCCESS);
	// Move to b
	EXPECT_EQ(
		behaviorTreeManager_.ExecuteIndex(id), 
		city::BehaviorTreeFlow::SUCCESS);
	// Wait at b
	do 
	{
		status = behaviorTreeManager_.ExecuteIndex(id);
		if (status != city::BehaviorTreeFlow::SUCCESS)
		{
			EXPECT_EQ(status, city::BehaviorTreeFlow::RUNNING);
		}
	} 
	while (status != city::BehaviorTreeFlow::SUCCESS);
}
