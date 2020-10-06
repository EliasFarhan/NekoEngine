#pragma once
#include <array>
#include <cstdint>
#include <cstring>

namespace neko
{
template<typename T>
T ConvertFromBinary(const std::array<std::uint8_t, sizeof(T)>& data)
{
  T result;
  auto* resultPtr = reinterpret_cast<std::uint8_t*>(&result);
  std::memcpy(resultPtr, data.data(), sizeof(T));
  return result;
}

template<typename T>
std::array<std::uint8_t, sizeof(T)> ConvertToBinary(T data)
{
  std::array<std::uint8_t, sizeof(T)> result{};
  const auto* dataPtr = reinterpret_cast<std::uint8_t*>(&data);
  std::memcpy(result.data(), dataPtr, sizeof(T));
  return result;

}
}
