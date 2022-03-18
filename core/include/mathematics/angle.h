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
#include "mathematics/const.h"



namespace neko
{

class Radian;

class Degree
{
public:
    constexpr Degree() = default;
    constexpr explicit Degree(float value): value_(value){}
    constexpr Degree(const Radian& angle); 
    [[nodiscard]] constexpr float GetValue() const { return value_; }

    constexpr Degree operator-() const
    {
        return Degree(-value_);
    }
    constexpr Degree operator*(float value) const
    {
        return Degree(value_ * value);
    }
    constexpr Degree operator/(float value) const
    {
        return Degree(value_ / value);
    }
    constexpr bool operator==(Degree other) const
    {
        return value_ == other.GetValue();
    }
private:
    float value_ = 0.0f;
};


class Radian
{
public:
    constexpr Radian() = default;
    constexpr explicit Radian(float value): value_(value){}
    constexpr Radian(const Degree& degree) : value_(degree.GetValue() / 180.0f * PI)
    {
    }
    [[nodiscard]] constexpr float GetValue() const { return value_; }

    constexpr Radian operator-() const
    {
        return Radian(-value_);
    }
    constexpr Radian operator/(float value) const
    {
        return Radian(value_ / value);
    }
    constexpr bool operator==(Radian other) const
    {
        return value_ == other.GetValue();
    }
private:
    float value_ = 0.0f;
};

constexpr Radian operator*(float value, Radian angle)
{
    return Radian(angle.GetValue() * value);
}

constexpr Degree::Degree(const Radian& radian) : value_(value_ = radian.GetValue() / PI * 180.0f)
{
}

inline float Sin(Radian radian) noexcept
{
    return std::sin(radian.GetValue());
}

inline float Cos(Radian radian) noexcept
{
    return std::cos(radian.GetValue());
}

inline float Tan(Radian radian) noexcept
{
    return std::tan(radian.GetValue());
}

inline Radian Asin(float ratio) noexcept
{
    return Radian(std::asin(ratio));
}

inline Radian Acos(float ratio) noexcept
{
    return Radian(std::acos(ratio));
}

inline Radian Atan(float ratio) noexcept
{
    return Radian(std::atan(ratio));
}

inline Radian Atan2(float y, float x) noexcept
{
    return Radian(std::atan2(y, x));
}

}
