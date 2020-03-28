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
#include <cstring>
#include <custom_container/string.h>

namespace neko
{
    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    String::String(Allocator& allocator): allocator_(allocator)
    {
        length_ = 0;
    }
    
    String::String(Allocator& allocator, std::string_view str): allocator_(allocator)
    {
        if (!str.empty())
        {
            buffer_ = static_cast<char*>(allocator_.Allocate(
	            sizeof(char) * str.size()+1,
	            alignof(char)));
            length_ = str.size()+1;

            for(int i = 0; i < length_-1; i++) {
                buffer_[i] = str.at(i);
            }
            buffer_[length_ - 1] = 0;
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
            for (unsigned i = 0; i < s.Length()-1; i++) {
                os << s.buffer_[i];
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
        allocator_.Deallocate(buffer_);
        length_ = rhs.Length();
        buffer_ = static_cast<char*>(allocator_.Allocate(
            sizeof(char) * length_ + 1,
            alignof(char)));

        for (unsigned i = 0; i < length_; i++) {
            buffer_[i] = rhs[i];
        }
        buffer_[length_ - 1] = 0;
        return *this;
    }

   String& String::operator=(const char rhs[]) {
        allocator_.Deallocate(buffer_);
        length_ = strlen(rhs)+1;

        buffer_ = static_cast<char*>(allocator_.Allocate(
            sizeof(char) * length_ + 1,
            alignof(char)));

        for(unsigned i = 0; i < length_;i++) {
            buffer_[i] = rhs[i];
        }

        buffer_[length_ - 1] = 0;
        return *this;
    }

    String& String::operator+= (const String& rhs)
    {
        unsigned len = length_-1 + rhs.Length();
        char* str = static_cast<char*>(allocator_.Allocate(
            sizeof(char) * length_ + 1,
            alignof(char)));
        
        for (unsigned i = 0; i < length_-1; i++) {
            str[i] = buffer_[i];
        }

        for (unsigned j = 0; j < rhs.Length(); j++) {
            str[length_-1 + j] = rhs[j];
        }

        allocator_.Deallocate(buffer_);
        length_ = len;
        buffer_ = str;
        return *this;
    }

    bool operator==(const String& lhs, const String& rhs) {
        return strcmp(lhs.buffer_, rhs.buffer_) == 0;
    }

    bool operator==(const String& lhs, const char rhs[]) {
        return strcmp(lhs.buffer_, rhs) == 0;
    }

    bool operator!=(const String& lhs, const String& rhs) {
        return !(lhs == rhs);
    }

    bool operator!=(const String& lhs, const char rhs[]) {
        return !(lhs == rhs);
    }
}