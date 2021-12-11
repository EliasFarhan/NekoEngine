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
#include <engine/log.h>
#include <spdlog/spdlog.h>

#if defined(__ANDROID__)
#include <android/log.h>
#endif
namespace neko
{

static std::vector<std::string> logs;
void logDebug(std::string_view msg)
{
#if defined(__ANDROID__)
    __android_log_print(ANDROID_LOG_INFO, "NekoEngine", "%s", msg.data());
#else
    spdlog::info(msg);
#endif
}

void logWarning(std::string_view msg)
{
#if defined(__ANDROID__)
    __android_log_print(ANDROID_LOG_WARN, "NekoEngine", "%s", msg.data());
#else
    spdlog::warn(msg);
#endif
}

void logError(std::string_view msg)
{
#if defined(__ANDROID__)
    __android_log_print(ANDROID_LOG_ERROR, "NekoEngine", "%s", msg.data());
#else
    spdlog::error(msg);
#endif
}

const std::vector<std::string>& getLog()
{
    return logs;
}
}