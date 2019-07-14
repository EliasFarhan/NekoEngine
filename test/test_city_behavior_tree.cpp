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
#include <City/city_behavior_tree.h>

class BehaviorTreeTest : public ::testing::Test, public neko::BasicEngine
{
protected:
	void SetUp() override 
	{
		Init();
	}

	void TearDown() override 
	{
		Destroy();
	}

	std::vector<std::pair<std::string, std::string>> ilVariables_ = 
	{
		{"name", "test"},
		{"condition", "test(2)"},
		{"to", "move(1.0, 2.0)"},
		{"delay", "0.5"},
		{"decorator", "decorator()"},
		{"functional", "functional(0.0, 1.0, 2.0, 3.0)"},
	};
	const neko::Index comp_ = 0xbeef;
	city::FunctionMap funcMap_ = city::FunctionMap(comp_);
};

TEST_F(BehaviorTreeTest, CompositeConstructorTest)
{
    city::BehaviorTreeComposite composite;
	EXPECT_EQ(
		composite.GetType(),
        city::BehaviorTreeObjectType::INTERFACE_COMPOSITE);
	EXPECT_EQ(composite.GetVariable("name"), "<null>");
}

TEST_F(BehaviorTreeTest, DecoratorConstructorTest)
{
	// Decorator function (will return the condition operator response).
	funcMap_.SetFunction(
		"decorator",
		[](neko::Index comp, const std::vector<double>& values)
	{
		EXPECT_EQ(0xbeef, comp);
		EXPECT_EQ(values.size(), 1);
		// Basically return what was send.
		if (values[0] ==
			static_cast<double>(city::BehaviorTreeFlow::SUCCESS))
		{
			return true;
		}
		return false;
	});
	// Condition function (will return true).
	funcMap_.SetFunction(
		"test",
		[](neko::Index comp, const std::vector<double>& values)
	{
		EXPECT_EQ(0xbeef, comp);
		EXPECT_EQ(values.size(), 1);
		return true;
	});
	city::BehaviorTreeDecorator decorator(
		comp_, 
		std::make_shared<city::BehaviorTreeLeafCondition>(
			city::BehaviorTreeLeafCondition(comp_, ilVariables_)),
		ilVariables_);
	EXPECT_EQ(
		decorator.GetType(), 
		city::BehaviorTreeObjectType::INTERFACE_DECORATOR);
	EXPECT_EQ(decorator.GetVariable("name"), "test");
	EXPECT_EQ(decorator.Execute(), city::BehaviorTreeFlow::SUCCESS);
}

TEST_F(BehaviorTreeTest, LeafConstructorTest)
{
	city::BehaviorTreeLeaf leaf;
	EXPECT_EQ(
		leaf.GetType(), 
		city::BehaviorTreeObjectType::INTERFACE_LEAF);
}

TEST_F(BehaviorTreeTest, ConditionConstructorTest)
{
	funcMap_.SetFunction(
		"test",
		[](neko::Index comp, const std::vector<double>& values)
	{
		EXPECT_EQ(0xbeef, comp);
		EXPECT_EQ(values.size(), 1);
		return true;
	});
	city::BehaviorTreeLeafCondition condition(comp_, ilVariables_);
	EXPECT_EQ(
		condition.GetType(), 
		city::BehaviorTreeObjectType::LEAF_CONDITION);
	EXPECT_EQ(condition.Execute(), city::BehaviorTreeFlow::SUCCESS);
}

TEST_F(BehaviorTreeTest, MoveToConstructorTest)
{
	funcMap_.SetFunction(
		"move",
		[](neko::Index comp, const std::vector<double>& values)
	{
		EXPECT_EQ(0xbeef, comp);
		EXPECT_EQ(values.size(), 2);
		EXPECT_EQ(values[0], 1.0);
		EXPECT_EQ(values[1], 2.0);
		return true;
	});
	city::BehaviorTreeLeafMoveTo moveTo(comp_, ilVariables_);
	EXPECT_EQ(
		moveTo.GetType(), 
		city::BehaviorTreeObjectType::LEAF_MOVE_TO);
	EXPECT_EQ(moveTo.Execute(), city::BehaviorTreeFlow::SUCCESS);
}

TEST_F(BehaviorTreeTest, FunctionalConstructorTest)
{
	funcMap_.SetFunction(
		"functional",
		[](neko::Index comp, const std::vector<double>& values)
	{
		EXPECT_EQ(0xbeef, comp);
		EXPECT_EQ(values.size(), 4);
		EXPECT_EQ(values[0], 0.0);
		EXPECT_EQ(values[1], 1.0);
		EXPECT_EQ(values[2], 2.0);
		EXPECT_EQ(values[3], 3.0);
		return true;
	});
	city::BehaviorTreeLeafFunctional functional(comp_, ilVariables_);
	EXPECT_EQ(
		functional.GetType(),
		city::BehaviorTreeObjectType::LEAF_FUNCTIONAL);
	EXPECT_EQ(functional.Execute(), city::BehaviorTreeFlow::SUCCESS);
}

TEST_F(BehaviorTreeTest, WaitConstructorTest)
{
	city::BehaviorTreeLeafWait wait(ilVariables_);
	EXPECT_EQ(
		wait.GetType(), 
		city::BehaviorTreeObjectType::LEAF_WAIT);
	const auto now = std::chrono::system_clock::now();
	city::BehaviorTreeFlow status;
	do 
	{
		status = wait.Execute();
		if (status != city::BehaviorTreeFlow::SUCCESS) 
		{
			EXPECT_EQ(status, city::BehaviorTreeFlow::RUNNING);
		}
	} 
	while (status != city::BehaviorTreeFlow::SUCCESS);
	const auto msdt = 
		std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now() - now);
	EXPECT_GE(
		msdt.count(), 
		static_cast<long long>(stod(wait.GetVariable("delay")) * 1000.0));
}

TEST_F(BehaviorTreeTest, SelectorConstructorTest)
{
	city::BehaviorTreeCompositeSelector selector(
		std::vector<std::shared_ptr<city::BehaviorTreeNode>>{
			std::make_shared<city::BehaviorTreeLeafCondition>(
				city::BehaviorTreeLeafCondition(comp_, ilVariables_)),
			std::make_shared<city::BehaviorTreeLeafCondition>(
				city::BehaviorTreeLeafCondition(comp_))},
		ilVariables_);
	EXPECT_EQ(
		selector.GetType(), 
		city::BehaviorTreeObjectType::COMPOSITE_SELECTOR);
	EXPECT_EQ(selector.Execute(), city::BehaviorTreeFlow::SUCCESS);
	// Should stick in the success.
	EXPECT_EQ(selector.Execute(), city::BehaviorTreeFlow::SUCCESS);
}

TEST_F(BehaviorTreeTest, SequenceConstructorTest)
{
	city::BehaviorTreeCompositeSequence sequence(
		std::vector<std::shared_ptr<city::BehaviorTreeNode>>{
			std::make_shared<city::BehaviorTreeLeafCondition>(
				city::BehaviorTreeLeafCondition(comp_, ilVariables_)),
			std::make_shared<city::BehaviorTreeLeafCondition>(
				city::BehaviorTreeLeafCondition(comp_))},
		ilVariables_);
	EXPECT_EQ(
		sequence.GetType(), 
		city::BehaviorTreeObjectType::COMPOSITE_SEQUENCE);
	EXPECT_EQ(sequence.Execute(), city::BehaviorTreeFlow::SUCCESS);
	EXPECT_EQ(sequence.Execute(), city::BehaviorTreeFlow::FAILURE);
}
