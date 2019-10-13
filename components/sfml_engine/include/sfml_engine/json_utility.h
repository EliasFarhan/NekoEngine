#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

using json = nlohmann::json;
namespace neko
{
namespace sfml
{
sf::Vector2f GetVectorFromJson(const json& jsonObject, std::string parameterName);

sf::IntRect GetIntRectFromJson(const json& jsonObject, std::string parameterName);

sf::FloatRect GetFloatRectFromJson(const json& jsonObject, std::string parameterName);
}
}