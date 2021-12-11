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

namespace neko
{
constexpr float Linear(float t)
{
    return t;
}

constexpr float SmoothStart2(float t)
{
    return t * t;
}

constexpr float SmoothStart3(float t)
{
    return t * t * t;
}

constexpr float SmoothStart4(float t)
{
    return t * t * t * t;
}

constexpr float SmoothStart5(float t)
{
    return t * t * t * t * t;
}

constexpr float FlipY(float t)
{
    return 1.0f - t;
}

constexpr float SmoothStop2(float t)
{
    const float inverseT = 1.0f - t;
    return 1.0f - inverseT * inverseT;
}

constexpr float SmoothStop3(float t)
{
    const float inverseT = 1.0f - t;
    return 1.0f - inverseT * inverseT * inverseT;
}

constexpr float SmoothStop4(float t)
{
    const float inverseT = 1.0f - t;
    return 1.0f - inverseT * inverseT * inverseT * inverseT;
}

constexpr float SmoothStop5(float t)
{
    const float inverseT = 1.0f - t;
    return 1.0f - inverseT * inverseT * inverseT * inverseT * inverseT;
}

constexpr float SmoothStep(float t)
{
    return t * t * (3.0f - 2.0f * t);
}

constexpr float SmootherStep(float t)
{
    return t * t * t * (6.0f * t * t - 15.0f * t + 10.0f);
}

constexpr float Mix(float a, float b, float blend)
{
    return (a + blend * (b - a));
}

constexpr float Scale(float a, float t)
{
    return  a * t;
}

constexpr float ReverseScale(float a, float t)
{
    return (1.0f - t) * a;
}

constexpr float Arch2(float t)
{
    return Scale(FlipY(t), t);
}

constexpr float SmoothStartArch3(float t)
{
    return 20.0f / 3.0f * Scale(Arch2(t), t);
}

constexpr float SmoothStopArch3(float t)
{
    return 20.0f / 3.0f * ReverseScale(Arch2(t), t);
}

constexpr float SmoothStepArch4(float t)
{
    return 12.0f / 5.0f * ReverseScale(SmoothStartArch3(t), t);
}

constexpr float BellCurve6(float t)
{
    return 36.0f / 25.0f * SmoothStopArch3(t) * SmoothStartArch3(t);
}

constexpr float NormalizedBezier3(float b, float c, float t)
{
    const float s = 1.0f - t;
    const float t2 = t * t;
    const float s2 = s * s;
    const float t3 = t2 * t;
    return (3.0f * b * s2 * t) + (3.0f * c * s * t2) + t3;
}
}