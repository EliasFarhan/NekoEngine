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

#include <engine/string.h>

namespace neko {
    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    String::String(FreeListAllocator& allocator): allocator_(allocator)
    {
        length = 0;
        allocator_.Allocate(sizeof(neko::String), alignof(neko::String));
    }
    
    String::String(FreeListAllocator& allocator, const char* c): allocator_(allocator)
    {
        if (c){
            unsigned n = 0;

            while (c[n] != '\0') {
                n++;
                length = n;
                data = new char[n];

                for (unsigned i = 0; i < n; i++) {
                    data[i] = c[i];
                }
            }

            allocator_.Allocate(sizeof(neko::String) + length, alignof(neko::String));
        }
        else
        {
            length = 0;
            data = new char[0];
            allocator_.Allocate(sizeof(neko::String), alignof(neko::String));
        }
    }

    String::~String()
    {
        delete[] data;
    }

    size_t String::Length() const
    {
        return length;
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
        if (rhs >= length) throw 1;
        return data[rhs];
    }

    char& String::operator[] (unsigned rhs)
    {
        if (rhs >= length) throw 1;
        return data[rhs];
    }

    String& String::operator= (const String& rhs)
    {
        if (this == &rhs) { return *this;}

        delete data;
        length = rhs.Length();
        data = new char[length];

        for (unsigned i = 0; i < length; i++) {
            data[i] = rhs[i];
        }

        return *this;
    }



    String& String::operator+= (const String& rhs)
    {
        unsigned len = length + rhs.Length();
        char* str = new char[len];

        for (unsigned i = 0; i < length; i++) {
            str[i] = data[i];
        }

        for (unsigned j = 0; j < rhs.Length(); j++) {
            str[length + j] = rhs[j];
        }

        delete data;
        length = len;
        data = str;
        return *this;
    }

   /* String operator+ (const String& lhs, const String& rhs)
    {
        return String(lhs) += rhs;
    }

    String operator+ (const String& lhs, const char* rhs)
    {
        return String(lhs) += String(rhs);
    }

    String operator+ (const char* lhs, const String& rhs)
    {
        return String(lhs) += rhs;
    }

    bool operator== (const String& lhs, const String& rhs)
    {
        if (lhs.Length() != rhs.Length()) {
            return false;
        }

        int cap = lhs.Length();
        int n = 0;
        while ((n < cap) && (lhs[n] == rhs[n])) {
            n++;
        }
        return (n == cap);
    }
    //TODO STRNCOMP

    bool operator== (const String& lhs, const char* rhs)
    {
        return (lhs == String(rhs));
    }

    bool operator== (const char* lhs, const String& rhs)
    {
        return (String(lhs) == rhs);
    }*/

}