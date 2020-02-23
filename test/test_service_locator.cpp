//
// Created by efarhan on 25.01.20.
//
#include <gtest/gtest.h>
#include "utilities/service_locator.h"

class ProutInterface
{
public:
    virtual void Execute() const = 0;
};

class NullProut : public ProutInterface
{
public:
    void Execute() const override
    {

    }

public:
};

class Prout : public ProutInterface
{
    void Execute() const override
    {
        std::cout << "Coucou from prout!\n";
    }
};

using ProutLocator = neko::Locator<ProutInterface, NullProut>;

TEST(Engine, TestServiceLocator)
{
    {
        const auto& prout = ProutLocator::get();
        prout.Execute();
    }
    Prout concreteProut;
    ProutLocator::provide(&concreteProut);
    {
        const auto& prout = ProutLocator::get();
        prout.Execute();
    }
}
