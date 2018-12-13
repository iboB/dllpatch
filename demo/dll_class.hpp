#pragma once

#if defined DLL_SOURCE
#   define DLL_API __declspec(dllexport)
#else
#   define DLL_API __declspec(dllimport)
#endif

class DLL_API dll_class
{
public:
    dll_class();

    void inc_data();

    int get_data() const;

private:
    int data;
};
