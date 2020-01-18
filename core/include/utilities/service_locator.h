#pragma once

#include <type_traits>

template<class T, class TNull>
class Locator
{
    static_assert(std::is_base_of<T, TNull>::value, "TNull has to be a subtype of T");
public:
    static void initialize() { service_ = &nullService_; }

    static T& get() { return *service_; }

    static void provide(T* service)
    {
        if (service == nullptr)
        {
            // Revert to null service.
            service_ = &nullService_;
        }
        else
        {
            service_ = service;
        }
    }

protected:
    static T* service_;
    static TNull nullService_;
};