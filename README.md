# dllpatch.exe

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT) [![Build status](https://ci.appveyor.com/api/projects/status/b5fj2ydh7ynyhkvx?svg=true)](https://ci.appveyor.com/project/iboB/dllpatch)

Created for my blog post [The Search for Auto-Loaded DLLs and Windows rpath](https://ibob.github.io/blog/2018/12/16/windows-rpath/)

Changes the name of a DLL linked with a binary. Used as a poor man's substitute for rpath on Windows. 

For example you can make `demo.exe` search for `foo.dll` in a relative path (`subdirectory\foo.dll`) or an absolute one (`c:\dir\foo.dll`).

The implementation is pretty simple (for now?). If only replaces DLL names if the new one can fit. To make it work with a long new name, you have to provide a long old name, renaming `foo.dll` to `something-extra-extra-long-because-i-need-the-space-for-a-long-path.dll`.

For an example demo see the enclosed `run-demo.bat`. 


