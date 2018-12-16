@echo off
cd build
md demo\Debug\rel
move demo\Debug\dll_with_extra_long_name.dll demo\Debug\rel\foo.dll
if exist demo\Debug\dll_with_extra_long_name.dll (
    echo Old dll still exists!
    cd ..
    exit /b 1
)
rem patching demo.exe to link with a dll in a relative path
Debug\dllpatch.exe demo\Debug\demo.exe -r dll_with_extra_long_name.dll rel\foo.dll
if %errorlevel% neq 0 (
    echo Error running dllpatch
    cd ..
    exit /b 1
)
rem you can now run demo.exe and it should work like this:
demo\Debug\demo.exe
if %errorlevel% neq 0 (
    echo Error running demo
    cd ..
    exit /b 1
)
cd ..
echo Success
