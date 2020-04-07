#pragma once
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
        std::generate(funcTable_.Begin(), funcTable_.End(), [this, &i]()
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