#pragma once

/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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

#include <string>
#include <memory>
 //Externals includes
#include <json.hpp>

using json = nlohmann::json;

#ifdef USE_SFML
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#endif

namespace neko
{
bool IsJsonValueNumeric(const json::value_type& jsonValue);

bool CheckJsonExists(const json& jsonObject, std::string parameterName);

bool CheckJsonParameter(const json& jsonObject, std::string parameterName, json::value_t expectedType);

bool CheckJsonNumber(const json& jsonObject, std::string parameterName);
#ifdef USE_SFML

#endif
json LoadJson(const std::string_view jsonPath);
}