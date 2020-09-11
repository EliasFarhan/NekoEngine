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
#include <functional>
#include <array>

namespace neko
{
template<typename T = float, size_t resolution = 512>
class FuncTable
{
public:
    FuncTable(T start, T end, std::function<T(T)> func) : start_(start), end_(end), func_(func)
    {

    }

    void GenerateTable()
    {
        size_t i = 0;
        std::generate(funcTable_.begin(), funcTable_.end(), [this, &i]()
        {
            T x = T(i) / T(resolution) * (end_ - start_);
            i++;
            return func_(x);
        });
    }

    T GetValue(T x) const
    {
        size_t index = size_t(x / (end_ - start_) * resolution);
        return funcTable_[index];
    }

private:
    T start_ = 0.0f;
    T end_ = 1.0f;
    std::array<T, resolution> funcTable_;
    std::function<T(T)> func_;
};
}