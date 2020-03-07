#pragma once
#include <iostream>

namespace neko
{

class String
{
public:
    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------

    String();
    String(char c);

    String(const char* c);

    String(const String& s);

    ~String();

    unsigned Length() const;

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------

    friend std::ostream& operator<< (std::ostream& so, const String& s);
    friend std::istream& operator>> (std::istream& so, String& s);

    char  operator[] (unsigned rhs) const;
    char& operator[] (unsigned rhs);

    String& operator= (const String& rhs);


    String& operator+= (const String& rhs);


    friend String operator+ (const String& lhs, const String& rhs);
    friend String operator+ (const String& lhs, char rhs);
    friend String operator+ (const String& lhs, const char* rhs);
    friend String operator+ (char lhs, const String& rhs);
    friend String operator+ (const char* lhs, const String& rhs);

    friend bool operator== (const String& lhs, const String& rhs);
    friend bool operator== (const String& lhs, char rhs);
    friend bool operator== (const String& lhs, const char* rhs);
    friend bool operator== (char lhs, const String& rhs);
    friend bool operator== (const char* lhs, const String& rhs);

private :
    char* data;
    unsigned length;
};

}
