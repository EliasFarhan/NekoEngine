#pragma once


#include <string_view>
#include <engine/custom_allocator.h>

namespace neko
{

class String
{
    char* buffer_ = nullptr;
    size_t length_ = 0;
    FreeListAllocator& allocator_;

public:
    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------

    /// \brief Empty String Constructor
    String(FreeListAllocator& allocator);

    /// \brief char* String Constructor
    String(FreeListAllocator& allocator, const std::string_view str);

    /// \brief Default String Destructor
    ~String();

    /// \brief String length
    size_t Length() const;


    //-----------------------------------------------------------------------------
    // Functions
    //-----------------------------------------------------------------------------
    
    /// \brief String to C String
    const char* c_str(const String& rhs);

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------

    friend std::ostream& operator<< (std::ostream& so, const String& s);

    char  operator[] (unsigned rhs) const;
    char& operator[] (unsigned rhs);

    String& operator= (const String& rhs);

    String& operator+= (const String& rhs);


    /*friend String operator+ (const String& lhs, const String& rhs);
    friend String operator+ (const String& lhs, const char* rhs);
    friend String operator+ (const char* lhs, const String& rhs);

    friend bool operator== (const String& lhs, const String& rhs);
    friend bool operator== (const String& lhs, const char* rhs);
    friend bool operator== (const char* lhs, const String& rhs);*/

    //TODO UTF-8
private :

};

}
