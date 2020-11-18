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

#include <utils/json_utility.h>
#include <utils/file_utility.h>
#include <fstream>
#include <string>
#include "engine/log.h"
#include <sstream>
#include <fmt/format.h>

namespace neko
{

bool IsJsonValueNumeric(const json::value_type& jsonValue)
{
    return jsonValue.type() == json::value_t::number_float ||
           jsonValue.type() == json::value_t::number_integer ||
           jsonValue.type() == json::value_t::number_unsigned;
}

bool CheckJsonExists(const json& jsonObject, std::string parameterName)
{
    return jsonObject.find(parameterName) != jsonObject.end();
}

bool CheckJsonParameter(const json& jsonObject, std::string parameterName, json::value_t expectedType)
{
    return CheckJsonExists(jsonObject, parameterName) && jsonObject[parameterName].type() == expectedType;
}

bool CheckJsonNumber(const json& jsonObject, std::string parameterName)
{
    return CheckJsonParameter(jsonObject, parameterName, json::value_t::number_float) or
           CheckJsonParameter(jsonObject, parameterName, json::value_t::number_integer) or
           CheckJsonParameter(jsonObject, parameterName, json::value_t::number_unsigned);
}

Vec2f GetVectorFromJson(const json& jsonObject, std::string parameterName)
{
    Vec2f vector = Vec2f();
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

json LoadJson(const std::string_view jsonPath)
{

    json jsonContent;
    if (!neko::FileExists(jsonPath))
    {
        logDebug(fmt::format("[Error] File does not exist: {}", jsonPath));
        return jsonContent;
    }

    const auto jsonStrContent = LoadFile(jsonPath.data());
    jsonContent = json::parse(jsonStrContent, nullptr, false);

    return jsonContent;
}

}
