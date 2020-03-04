#pragma once

#include <cstdlib>
#include <iostream>

#ifdef NEKO_ASSERT
#define neko_assert(Expr, Msg) \
    if(!(Expr)) \
    { \
        std::cerr << "Assert failed:\t"<<Msg <<'\n' \
            <<"Condition:\t"<< #Expr << '\n' \
            << "Source:\t\t"<<__FILE__<<", line "<<__LINE__<<'\n'; \
            abort(); \
    }
#else
#define neko_assert(Expr, Msg) \
    void(0);
#endif
