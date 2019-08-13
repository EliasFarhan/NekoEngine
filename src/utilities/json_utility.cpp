/*
MIT License

Copyright (c) 2019 SAE Institute Switzerland AG

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

#include <utilities/json_utility.h>

#include <fstream>
#include <string>
#include "engine/log.h"
#include <sstream>

namespace neko
{

bool IsJsonValueNumeric(const json::value_type & jsonValue)
{
	return jsonValue.type() == json::value_t::number_float || 
		   jsonValue.type() == json::value_t::number_integer || 
		   jsonValue.type() == json::value_t::number_unsigned;
}

bool CheckJsonExists(const json & jsonObject, std::string parameterName)
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

json LoadJson(const std::string& jsonPath)
{

    json jsonContent;
	std::ifstream jsonFile(jsonPath.c_str());
	if (jsonFile.peek() == std::ifstream::traits_type::eof())
	{
		{
			std::ostringstream oss;
			oss << "[Error] Empty json file at: " << jsonPath;
			logDebug(oss.str());
		}
		return jsonContent;
	}
	try
	{
		jsonFile >> jsonContent;
	}
	catch (json::parse_error& e)
	{
		{
			std::ostringstream oss;
			oss << "[Error] File: " << jsonPath << " is not json\n" << e.what();
			logDebug(oss.str());
		}
		return nullptr;
	}
	return jsonContent;
}

sf::IntRect GetIntRectFromJson(const json &jsonObject, std::string parameterName)
{
    sf::IntRect rect;
    try
    {
        if (CheckJsonParameter(jsonObject, parameterName, json::value_t::array))
        {
            rect = {jsonObject[parameterName][0],
                    jsonObject[parameterName][1],
                    jsonObject[parameterName][2],
                    jsonObject[parameterName][3]};
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
