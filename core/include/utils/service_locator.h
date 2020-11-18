#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

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
#include <type_traits>

namespace neko
{
template<class T, class TNull>
class Locator
{
    static_assert(std::is_base_of<T, TNull>::value, "TNull has to be a subtype of T");
public:

    static T& get()
    { return *service_; }

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

template <class T, class TNull> TNull Locator<T, TNull> ::nullService_;
template <class T, class TNull> T* Locator<T, TNull> ::service_ = &Locator<T, TNull>::nullService_;

}