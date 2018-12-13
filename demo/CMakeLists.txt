add_library(dll_with_extra_long_name SHARED
    dll_class.hpp
    dll_class.cpp
)

add_executable(demo demo.cpp)
target_link_libraries(demo dll_with_extra_long_name)
