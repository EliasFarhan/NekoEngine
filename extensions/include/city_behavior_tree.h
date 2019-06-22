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

// Object type enum this will give witch type an object is.
enum CompositeObjectType
{
    // Basic types these are not suppose to be in a behavior tree.
            INTERFACE_COMPOSITE,
    INTERFACE_DECORATOR,
    INTERFACE_LEAF,
    // Composite type these are suppose to be in a behavior tree.
            COMPOSITE_SEQUENCE,
    COMPOSITE_SELECTOR,
    // Leaf type also suppose to be in behavior tree.
            LEAF_WAIT,
    LEAF_MOVE_TO,
};

// Object type list used to parse the behavior tree in json format.
const static std::map<CompositeObjectType, std::string> mapCompositeString = {
        {INTERFACE_COMPOSITE, "interface_composite"},
        {INTERFACE_DECORATOR, "interface_decorator"},
        {INTERFACE_LEAF,      "interface_leaf"},
        // Composite are connected to more than one sub element derives from Composite.
        {COMPOSITE_SEQUENCE,  "composite_sequence"},
        {COMPOSITE_SELECTOR,  "composite_selector"},
        // Leaf component that derives from leaf node.
        {LEAF_WAIT,           "leaf_wait"},
        {LEAF_MOVE_TO,        "leaf_move_to"},
};

// Global interface for a node in a behavior tree.
class BehaviorTreeNode
{
public:
    virtual BehaviorTreeFlow Execute() = 0;

    virtual CompositeObjectType GetType() const = 0;

    virtual void SetVariable(const std::string& variable, const std::string& value);

    virtual std::string GetVariable(const std::string& variable) const;

protected:
    std::map<std::string, std::string> variables_;
};

// Decorator in a behavior tree.
class BehaviorTreeDecorator : public BehaviorTreeNode
{
public:
    virtual BehaviorTreeFlow Execute() override
    {}

    virtual CompositeObjectType GetType() const
    { return INTERFACE_DECORATOR; }

protected:
    std::shared_ptr<BehaviorTreeNode> child_;
};

// Composite in a behavior tree.
class BehaviorTreeComposite : public BehaviorTreeNode
{
public:
    virtual BehaviorTreeFlow Execute() override
    {}

    virtual CompositeObjectType GetType() const
    { return INTERFACE_COMPOSITE; }

protected:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children_;
    uint32_t currentCount_;
};

// Leaf in a behavior tree.
class BehaviorTreeLeaf : public BehaviorTreeNode
{
public:
    virtual BehaviorTreeFlow Execute() override
    {}

    virtual CompositeObjectType GetType() const
    { return INTERFACE_LEAF; }
};

// Sequence in a behavior tree.
class BehaviorTreeComponentSequence : public BehaviorTreeComposite
{
public:
    virtual BehaviorTreeFlow Execute() final;

    virtual CompositeObjectType GetType() const final
    { return COMPOSITE_SEQUENCE; }
};

// Selector in a behavior tree.
class BehaviorTreeComponentSelector : public BehaviorTreeComposite
{
public:
    virtual BehaviorTreeFlow Execute() final;

    virtual CompositeObjectType GetType() const final
    { return COMPOSITE_SELECTOR; }
};

// Leaf Wait in a behavior tree.
// The delay can be set with the SetVariable("delay", "1.0");.
class BehaviorTreeWait : public BehaviorTreeLeaf
{
public:
    virtual BehaviorTreeFlow Execute() final;

    virtual CompositeObjectType GetType() const final
    { return LEAF_WAIT; }

private:
    Timer timer_;
    bool started_ = false;
};

// Leaf Move to in a behavior tree.
// The location can be set by the SetVariable("to", "10.0, 20");.
class BehaviorTreeLeafMoveTo : public BehaviorTreeLeaf
{
public:
    virtual BehaviorTreeFlow Execute() final;

    virtual CompositeObjectType GetType() const final
    { return LEAF_MOVE_TO; }
};

}    // namespace neko