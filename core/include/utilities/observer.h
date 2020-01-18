#pragma once

#include <type_traits>

namespace neko
{

class SubjectInterface
{

};

template <class T>
class ObserverInterface
{
    static_assert(std::is_base_of<T, SubjectInterface>::value);
};
}