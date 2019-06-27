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
#include <city_behavior_tree.h>

class BehaviorTreeTest : public ::testing::Test, public neko::BasicEngine {
protected:
	void SetUp() override 
	{
		Init();
		comp_ = std::make_shared<neko::Component>();
		passed_ = 0;
		neko::FunctionMap funcMap(comp_);
		funcMap.SetFunction(
			"test",
			[this](std::shared_ptr<neko::Component> comp, double value)
		{
			passed_++;
			EXPECT_EQ(value, 2.0);
			EXPECT_EQ(*comp_, *comp);
			return true;
		});
	}

	void TearDown() override 
	{
		Destroy();
	}

	std::vector<std::pair<std::string, std::string>> ilVariables_ = {
		{"name", "test"},
		{"condition", "test(2)"},
		{"delay", "0.5"},
	};
	int passed_;
	std::shared_ptr<neko::Component> comp_;
};

TEST_F(BehaviorTreeTest, CompositeConstructorTest)
{
	neko::BehaviorTreeComposite composite;
	EXPECT_EQ(composite.GetType(), neko::INTERFACE_COMPOSITE);
	EXPECT_EQ(composite.GetVariable("name"), "<null>");
}

TEST_F(BehaviorTreeTest, DecoratorConstructorTest)
{
	neko::BehaviorTreeDecorator decorator{ ilVariables_ };
	EXPECT_EQ(decorator.GetType(), neko::INTERFACE_DECORATOR);
	EXPECT_EQ(decorator.GetVariable("name"), "test");
	// TODO moar tests!!!
}

TEST_F(BehaviorTreeTest, LeafConstructorTest)
{
	neko::BehaviorTreeLeaf leaf;
	EXPECT_EQ(leaf.GetType(), neko::INTERFACE_LEAF);
}

TEST_F(BehaviorTreeTest, ConditionConstructorTest)
{
	neko::BehaviorTreeLeafCondition condition(comp_, ilVariables_);
	EXPECT_EQ(condition.GetType(), neko::LEAF_CONDITION);
	EXPECT_EQ(condition.Execute(), neko::SUCCESS);
	EXPECT_EQ(passed_, 1);
}

TEST_F(BehaviorTreeTest, MoveToConstructorTest)
{
	neko::BehaviorTreeLeafMoveTo moveTo(ilVariables_);
	EXPECT_EQ(moveTo.GetType(), neko::LEAF_MOVE_TO);
	// TODO moar tests!!!
}

TEST_F(BehaviorTreeTest, WaitConstructorTest)
{
	neko::BehaviorTreeLeafWait wait(ilVariables_);
	EXPECT_EQ(wait.GetType(), neko::LEAF_WAIT);
	// TODO moar tests!!!
}

TEST_F(BehaviorTreeTest, SelectorConstructorTest)
{
	neko::BehaviorTreeCompositeSelector selector(
		std::vector<std::shared_ptr<neko::BehaviorTreeNode>>{
		std::make_shared<neko::BehaviorTreeLeafCondition>(
			neko::BehaviorTreeLeafCondition(comp_, ilVariables_)),
			std::make_shared<neko::BehaviorTreeLeafCondition>(
				neko::BehaviorTreeLeafCondition(comp_))},
		ilVariables_);
	EXPECT_EQ(selector.GetType(), neko::COMPOSITE_SELECTOR);
	EXPECT_EQ(selector.Execute(), neko::SUCCESS);
	EXPECT_EQ(passed_, 1);
	// Should stick in the success.
	EXPECT_EQ(selector.Execute(), neko::SUCCESS);
	EXPECT_EQ(passed_, 2);
}

TEST_F(BehaviorTreeTest, SequenceConstructorTest)
{
	neko::BehaviorTreeCompositeSequence sequence(
		std::vector<std::shared_ptr<neko::BehaviorTreeNode>>{
		std::make_shared<neko::BehaviorTreeLeafCondition>(
			neko::BehaviorTreeLeafCondition(comp_, ilVariables_)),
			std::make_shared<neko::BehaviorTreeLeafCondition>(
				neko::BehaviorTreeLeafCondition(comp_))},
		ilVariables_);
	EXPECT_EQ(sequence.GetType(), neko::COMPOSITE_SEQUENCE);
	EXPECT_EQ(sequence.Execute(), neko::SUCCESS);
	EXPECT_EQ(passed_, 1);
	EXPECT_EQ(sequence.Execute(), neko::FAILURE);
	EXPECT_EQ(passed_, 2);
}
