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
#include <cassert>

using namespace std;

const char* version = "1.00";

void print_version()
{
    cout << "\ndllpatch " << version << "\n\n";
}

void print_usage()
{
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
    print_version();
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
        "         Patches myexe to link against foo\\foo.dll instead of foo.dll.\n"
        "         Writes output to myexe_patched.exe.\n"
        "\n";
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        print_usage();
        return 0;
    }

    const char* exe_name = nullptr;
    const char* output_exe_name = nullptr;
    ifstream fin;
    vector<char> exe;
    for (int i=1; i<argc; ++i)
    {
        const string arg = argv[i];

        if (arg == "-v" || arg == "--version")
        {
            print_version();
            return 0;
        }

        if (arg == "--help" || arg == "-h" || arg == "-?")
        {
            print_help();
            return 0;
        }

        if (!exe_name)
        {
            exe_name = argv[i];
            fin.open(exe_name, ios::in | ios::binary);
            if (!fin)
            {
                cerr << "Error: cannot open input " << exe_name << "\n";
                suggest_help();
                return 1;
            }

            fin.seekg(0, ios::end);
            auto size = fin.tellg();
            exe.resize(size_t(size));
            fin.seekg(0, ios::beg);
            fin.read(exe.data(), size);
            fin.close();
            continue;
        }

        if (arg == "--output" || arg == "-o")
        {
            if (i == argc - 1)
            {
                cout << "Error: missing output file\n";
                suggest_help();
                return 1;
            }

            output_exe_name = argv[++i];
            continue;
        }

        if (arg == "--rename" || arg == "-r")
        {
            if (i + 2 >= argc)
            {
                cout << "Error: rename requires two DLL names: old and new\n";
                suggest_help();
                return 1;
            }

            const string dll_name = argv[++i];
            const auto it = std::search(exe.begin(), exe.end(), dll_name.begin(), dll_name.end());
            if (it == exe.end())
            {
                cerr << "Error: " << exe_name << " is not linked with " << dll_name << "\n";
                return 1;
            }

            auto end = it + dll_name.length();
            const auto second_pass = std::search(end, exe.end(), dll_name.begin(), dll_name.end());
            if (second_pass != exe.end())
            {
                cerr << "Error: the DLL name " << dll_name << " is found multiple times in "
                    << exe_name << ". It's not safe to proceed\n";
                return 1;
            }

            if (*end != 0)
            {
                cerr << "Error: " << dll_name << " was found in " << exe_name
                    << ", but it was not a standalone string. It's not safe to proceed\n";
                return 1;
            }

            // find padding
            while(!*end++);
            --end; // leave room for terminating zero

            auto max_new_name_length = end - it;

            const string new_dll_name = argv[++i];
            if (new_dll_name.length() > size_t(max_new_name_length))
            {
                cerr << "Error: there is no room in " << exe_name << " to replace "
                    << dll_name << " with " << new_dll_name
                    << ". Consider linking with a longer DLL name to make room.\n";
                return 1;
            }

            end = std::copy(new_dll_name.begin(), new_dll_name.end(), it);
            do
            { // fill the rest with zeroes so it would appear as padding on subsequent patches
                *end = 0;
            } while (*++end);

            cout << "Patched " << dll_name << " to " << new_dll_name << "\n";

            continue;
        }
    }
    fin.close();

    if (!output_exe_name)
    {
        output_exe_name = exe_name;
    }

    ofstream fout(output_exe_name, ios::out | ios::binary);
    fout.write(exe.data(), exe.size());

    return 0;
}
