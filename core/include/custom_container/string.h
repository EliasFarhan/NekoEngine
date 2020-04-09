#pragma once


#include <string_view>
#include <engine/custom_allocator.h>

namespace neko
{

class String
{   
    char* buffer_ = nullptr;
    size_t length_ = 0;
    Allocator& allocator_;

public:
    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------

    /// \brief Empty String Constructor
    String(Allocator& allocator);

    /// \brief char* String Constructor
    String(Allocator& allocator, const std::string_view str);

    /// \brief Default String Destructor
    ~String();

    /// \brief String length
    size_t Length() const;

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------

    friend std::ostream& operator<< (std::ostream& os, const String& s);

    char  operator[] (unsigned rhs) const;
    char& operator[] (unsigned rhs);

    String& operator= (const String& rhs);
    String& operator= (const char rhs[]);

    String& operator+= (const String& rhs);

    friend bool operator== (const String& lhs, const String& rhs);
    friend bool operator== (const String& lhs, const char rhs[]);

    friend bool operator!= (const String& lhs, const String& rhs);
    friend bool operator!= (const String& lhs, const char rhs[]);

    //TODO UTF-8
private :
    const size_t baseAllocatedSize_ = 128;
};

}
