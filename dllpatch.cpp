// Copyright(c) 2018 Borislav Stanimirov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const char* version = "1.00";

void print_version()
{
    cout << "dllpatch " << version << "\n\n";
}

void print_usage()
{
    print_version();
    cout <<
        "Usage:\n"
        "    dllpatch.exe <input exe> -r <old DLL> <new DLL> [-o <output exe>]\n"
        "\n";
}

void suggest_help()
{
    cout <<
        "For help:\n"
        "    dllpatch.exe --help\n"
        "\n";
}

void print_help()
{
    print_usage();

    cout <<
        "Help:\n"
        "    -r <old DLL> <new DLL>\n"
        "         Rename a DLL against which the executable is linked to another.\n"
        "         Multiple renames with a single execution are possible.\n"
        "    -o <output exe>\n"
        "         Optionally set the output executable. Outputs in place if not set.\n"
        "    --help | -h | -?\n"
        "         Prints this help message. Ignores other arguments.\n"
        "    --version | -v\n"
        "         Prints version. Ignores other arguments.\n"
        "\n"
        "Example:\n"
        "    dllpatch.exe myexe.exe -r foo.dll foo\\foo.dll -o myexe_patched.exe\n"
        "         Patches myexe in place to link against foo\\foo.dll instead of foo.dll.\n"
        "\n";
}

int main(int argc, char* argv[])
{
    print_help();
    return 0;
    const char* exe_name = "demo/Release/demo.exe";
    ifstream fin(exe_name, ios::in | ios::binary);
    if (!fin.is_open())
    {
        cerr << "Error: canot open " << exe_name << "\n";
        return 1;
    }
    fin.seekg(0, ios::end);
    auto size = fin.tellg();
    fin.seekg(0, ios::beg);

    vector<char> exe((size_t)size);
    fin.read(exe.data(), size);
    fin.close();

    string dll_name = "dll_with_extra_long_name.dll";

    auto it = std::search(exe.begin(), exe.end(), dll_name.begin(), dll_name.end());
    if (it == exe.end())
    {
        cerr << "Error: " << exe_name << " is not linked with " << dll_name << "\n";
        return 1;
    }

    string new_dll_name = "temp\\d.dll";
    it = std::copy(new_dll_name.begin(), new_dll_name.end(), it);
    *it = 0;

    const char* new_exe_name = "demo/Release/demo_p.exe";
    ofstream fout(new_exe_name, ios::out | ios::binary);
    fout.write(exe.data(), exe.size());
    return 0;
}
