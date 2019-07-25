#pragma once
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

#include <functional>
#include <map>
#include <string>
#include <engine/globals.h>

namespace city {

	class FunctionMap {
	public:
		FunctionMap(neko::Index componentId = neko::INVALID_INDEX) : comp_(componentId) {}
		void SetFunction(
			const std::string_view name, 
			std::function<bool(neko::Index, const std::vector<double>&)> func);
		bool CallFunction(const std::string_view name, double value);
		bool CallFunction(
			const std::string_view name, 
			const std::vector<double>& vecDouble);

	protected:
        neko::Index comp_;
		static std::map<
			std::string, 
			std::function<bool(neko::Index, const std::vector<double>&)>>
			staticNameFunctionMap_;
	};

} // end namespace neko