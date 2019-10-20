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

#include <city/function_map.h>
#include <engine/log.h>

namespace neko {

	std::map<
		std::string, 
		std::function<bool(Index, const std::vector<double>&)>>
		FunctionMap::staticNameFunctionMap_;

	void FunctionMap::SetFunction(
		const std::string_view name,
		std::function<bool(Index, const std::vector<double>&)> func)
	{
		staticNameFunctionMap_.insert(std::make_pair(std::string(name), func));
	}

	bool FunctionMap::CallFunction(const std::string_view name, double value)
	{
		auto it = staticNameFunctionMap_.find(std::string(name));
		if (it != staticNameFunctionMap_.end())
		{
			return it->second(comp_, { value });
		}
		logDebug("ERROR executing : " + std::string(name));
		return false;
	}

	bool FunctionMap::CallFunction(
		const std::string_view name, 
		const std::vector<double>& values)
	{
		auto it = staticNameFunctionMap_.find(std::string(name));
		if (it != staticNameFunctionMap_.end())
		{
			return it->second(comp_, values);
		}
		logDebug("ERROR executing : " + std::string(name));
		return false;
	}

} // end namespace neko