#define DLL_SOURCE
#include "dll_class.hpp"

dll_class::dll_class()
    : data(572)
{
}

void dll_class::inc_data()
{
    ++data;
}

int dll_class::get_data() const
{
    return data;
}
