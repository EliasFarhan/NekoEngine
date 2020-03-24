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

#include <custom_container/string.h>

namespace neko
{
    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    String::String(FreeListAllocator& allocator): allocator_(allocator)
    {
        length_ = 0;
    }
    
    String::String(FreeListAllocator& allocator, std::string_view str): allocator_(allocator)
    {
        if (!str.empty())
        {
            buffer_ = static_cast<char*>(allocator_.Allocate(
	            sizeof(char) * str.size(),
	            alignof(char)));
            length_ = str.size();

            char* cstr = new char[length_];

            for (unsigned i = 0; i < length_; i++) {
                cstr[i] = buffer_[i];
            }

            cstr_ = cstr;
        }

    }

    String::~String()
    {
    	if(buffer_ != nullptr)
    	{
            allocator_.Deallocate(buffer_);
            length_ = 0;
    	}
    }

    size_t String::Length() const
    {
        return length_;
    }

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------
    
    std::ostream& operator<< (std::ostream& os, const String& s)
    {
        if (s.Length() > 0)
        {
            for (unsigned i = 0; i < s.Length(); i++) {
                os << s[i];
            }
        }
        else os << "";

        return os;
    }

    char String::operator[] (unsigned rhs) const
    {
        if (rhs >= length_) throw 1;
        return buffer_[rhs];
    }

    char& String::operator[] (unsigned rhs)
    {
        if (rhs >= length_) throw 1;
        return buffer_[rhs];
    }

    String& String::operator= (const String& rhs)
    {
        if (this == &rhs) { return *this;}

        delete buffer_;
        length_ = rhs.Length();
        buffer_ = new char[length_];

        for (unsigned i = 0; i < length_; i++) {
            buffer_[i] = rhs[i];
        }

        return *this;
    }



    String& String::operator+= (const String& rhs)
    {
        unsigned len = length_ + rhs.Length();
        char* str = new char[len];

        for (unsigned i = 0; i < length_; i++) {
            str[i] = buffer_[i];
        }

        for (unsigned j = 0; j < rhs.Length(); j++) {
            str[length_ + j] = rhs[j];
        }

        delete buffer_;
        length_ = len;
        buffer_ = str;
        return *this;
    }

    bool operator==(const String& lhs, const String& rhs) {
        return strcmp(lhs.cstr_, rhs.cstr_);
    }
}