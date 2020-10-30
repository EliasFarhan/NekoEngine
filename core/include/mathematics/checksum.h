#pragma once
#include <vector>
#include <numeric>
#include <cstring>

namespace neko
{
template<typename returnT, typename T>
returnT Checksum(T arg)
{
    std::vector<returnT> result;
    result.resize(sizeof(T) / sizeof(returnT) + 1, 0);
    const auto* tPtr = reinterpret_cast<std::uint8_t*>(&arg);
    auto* resultPtr = reinterpret_cast<std::uint8_t*>(result.data());
    std::memcpy(resultPtr, tPtr, sizeof(T));
    return std::accumulate(result.cbegin(), result.cend(), 0);
}
}
