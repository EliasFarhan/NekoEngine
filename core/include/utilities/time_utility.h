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

#include <cstdint>
#include <chrono>

namespace neko
{
    using seconds = std::chrono::duration<float, std::ratio<1,1>>;
    using milliseconds = std::chrono::duration<std::uint32_t, std::ratio<1,1000>>;
	using microseconds = std::chrono::duration<std::uint32_t, std::ratio<1, 1'000'000>>;

/**
 * \brief simple timer class in second, that needs to be updated with the current delta time of the frame
 * period is how long will the timer will run
 * time is the current time from period to 0
 */
class Timer
{
public:
    Timer(seconds time, seconds period);

    void Update(seconds dt);

    bool IsOver() const;

    void Reset();

    float GetCurrentRatio() const;

    seconds GetTimeFromStart() const;


    seconds time{0.0f};
    seconds period {0.0f};
private:
};

void SleepMs(const milliseconds ms);

}
