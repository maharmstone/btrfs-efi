SET(CMAKE_SYSTEM_NAME Windows)

SET(CMAKE_C_COMPILER clang)
SET(CMAKE_SYSTEM_PROCESSOR ARM)

SET(CLANG_TARGET_TRIPLE armv7l-w64-mingw32)
SET(CMAKE_C_COMPILER_TARGET armv7l-w64-mingw32)

set(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

