//
// Created by efarhan on 13/10/2019.
//
#include <sstream>
#include <engine/log.h>
#include "utilities/json_utility.h"
#include "sfml_engine/json_utility.h"

namespace neko
{
namespace sfml
{
sf::Vector2f GetVectorFromJson(const json & jsonObject, std::string parameterName)
{
    sf::Vector2f vector = sf::Vector2f();
    if (CheckJsonParameter(jsonObject, parameterName, json::value_t::array))
    {
        if (jsonObject[parameterName].size() == 2)
        {
            auto vectorJson = jsonObject[parameterName];
            if (IsJsonValueNumeric(vectorJson[0]))
            {
                vector.x = vectorJson[0];
            }
            if (IsJsonValueNumeric(vectorJson[1]))
            {
                vector.y = vectorJson[1];
            }
        }
    }
    else if (CheckJsonParameter(jsonObject, parameterName, json::value_t::object))
    {
        auto vectorJson = jsonObject[parameterName];
        if (IsJsonValueNumeric(vectorJson["x"]))
        {
            vector.x = vectorJson["x"];
        }
        if (IsJsonValueNumeric(vectorJson["y"]))
        {
            vector.y = vectorJson["y"];
        }
    }
    return vector;
}

sf::IntRect GetIntRectFromJson(const json &jsonObject, std::string parameterName)
{
    sf::IntRect rect;
    try
    {
        if (CheckJsonParameter(jsonObject, parameterName, json::value_t::array))
        {
            rect = { jsonObject[parameterName][0],
                     jsonObject[parameterName][1],
                     jsonObject[parameterName][2],
                     jsonObject[parameterName][3] };
        }
    }
    catch (json::type_error& e)
    {
        std::ostringstream oss;
        oss << "[Error] Input is not rect: " << parameterName << "\n" << e.what();
        logDebug(oss.str());
    }
    return rect;
}

sf::FloatRect GetFloatRectFromJson(const json& jsonObject, std::string parameterName)
{
    sf::FloatRect rect;
    try
    {
        if (CheckJsonParameter(jsonObject, parameterName, json::value_t::array))
        {
            rect = { jsonObject[parameterName][0],
                     jsonObject[parameterName][1],
                     jsonObject[parameterName][2],
                     jsonObject[parameterName][3] };
        }
    }
    catch (json::type_error& e)
    {
        std::ostringstream oss;
        oss << "[Error] Input is not rect: " << parameterName << "\n" << e.what();
        logDebug(oss.str());
    }
    return rect;
}
}
}