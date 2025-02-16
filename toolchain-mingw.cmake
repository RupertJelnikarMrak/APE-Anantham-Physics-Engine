set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# vcpkg integration
set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")
set(VCPKG_CRT_LINKAGE "static")
set(VCPKG_LIBRARY_LINKAGE "static")

include("/home/rupert/.local/share/vcpkg/scripts/buildsystems/vcpkg.cmake")
