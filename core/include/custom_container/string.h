#pragma once


#include <string_view>
#include <engine/custom_allocator.h>

namespace neko
{

class NekoString
{   
    char* buffer_ = nullptr;
    size_t length_ = 0;
    Allocator& allocator_;

public:
    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------

    /// \brief Empty String Constructor
    NekoString(Allocator& allocator);

    /// \brief char* String Constructor
    NekoString(Allocator& allocator, const std::string_view str);

    /// \brief Default String Destructor
    ~NekoString();

    /// \brief String length
    size_t Length() const;

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------

    friend std::ostream& operator<< (std::ostream& os, const NekoString& s);

    char  operator[] (unsigned rhs) const;
    char& operator[] (unsigned rhs);

    NekoString& operator= (const NekoString& rhs);
    NekoString& operator= (const char rhs[]);

    NekoString& operator+= (const NekoString& rhs);

    friend bool operator== (const NekoString& lhs, const NekoString& rhs);
    friend bool operator== (const NekoString& lhs, const char rhs[]);

    friend bool operator!= (const NekoString& lhs, const NekoString& rhs);
    friend bool operator!= (const NekoString& lhs, const char rhs[]);

    //TODO UTF-8
private :
    const size_t baseAllocatedSize_ = 128;
};

}
