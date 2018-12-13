#include <iostream>

#include "dll_class.hpp"

using namespace std;

int main()
{
    dll_class x;
    x.inc_data();
    cout << "dll data is " << x.get_data() << "\n";
    return 0;
}
