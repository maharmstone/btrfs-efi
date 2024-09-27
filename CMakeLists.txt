cmake_minimum_required(VERSION 3.14)

project(btrfs-efi LANGUAGES C VERSION 20230328)

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:efi_main")
    add_compile_options("/GS-")
    string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")

    # work around bug in Visual Studio
    if (MSVC_CXX_ARCHITECTURE_ID STREQUAL "X86")
        set(CMAKE_SYSTEM_PROCESSOR "X86")
    endif()
else()
    add_compile_options(-fno-stack-check -fno-stack-protector -mno-stack-arg-probe)
endif()

include_directories(${CMAKE_SOURCE_DIR}/gnu-efi/inc)
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    include_directories(${CMAKE_SOURCE_DIR}/gnu-efi/inc/x86_64)
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "X86")
    include_directories(${CMAKE_SOURCE_DIR}/gnu-efi/inc/ia32)
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    include_directories(${CMAKE_SOURCE_DIR}/gnu-efi/inc/aarch64)
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
    include_directories(${CMAKE_SOURCE_DIR}/gnu-efi/inc/arm)
endif()

# -----------------------------------

set(ZSTD_SRC_FILES src/zstd/lib/common/entropy_common.c
    src/zstd/lib/common/error_private.c
    src/zstd/lib/common/fse_decompress.c
    src/zstd/lib/decompress/huf_decompress.c
    src/zstd/lib/common/zstd_common.c
    src/zstd/lib/decompress/zstd_ddict.c
    src/zstd/lib/decompress/zstd_decompress.c
    src/zstd/lib/decompress/zstd_decompress_block.c
    src/zstd/lib/common/xxhash.c)

add_library(zstd STATIC ${ZSTD_SRC_FILES})
target_compile_definitions(zstd PRIVATE -DZSTD_DEPS_MALLOC -DXXH_NO_STDLIB -DZSTD_HAVE_WEAK_SYMBOLS=0)

if(NOT MSVC)
    target_compile_options(zstd PRIVATE -ffunction-sections -include ${CMAKE_SOURCE_DIR}/src/zstd-shim.h)
else()
    target_compile_options(zstd PRIVATE /Gy /FI ${CMAKE_SOURCE_DIR}/src/zstd-shim.h)
endif()

# -----------------------------------

set(ZLIB_SRC_FILES src/zlib/adler32.c
    src/zlib/deflate.c
    src/zlib/inffast.c
    src/zlib/inflate.c
    src/zlib/inftrees.c
    src/zlib/trees.c
    src/zlib/zutil.c)

add_library(zlib STATIC ${ZLIB_SRC_FILES})
target_compile_definitions(zlib PRIVATE -DNO_GZIP -DZ_SOLO)

if(NOT MSVC)
    target_compile_options(zlib PRIVATE -ffunction-sections)
else()
    target_compile_options(zlib PRIVATE /Gy)
endif()

# -----------------------------------

set(SRC_FILES src/btrfs.c
    src/crc32c.c
    src/misc.c
    src/lzo.c)

add_executable(btrfs ${SRC_FILES})
target_link_libraries(btrfs zstd zlib)

set_target_properties(btrfs PROPERTIES SUFFIX ".efi")

if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    target_compile_options(btrfs PRIVATE "-ffreestanding")
    target_compile_options(btrfs PRIVATE "-fno-stack-protector")
    target_compile_options(btrfs PRIVATE "-fno-stack-check")
    target_compile_options(btrfs PRIVATE "-mno-stack-arg-probe")
    target_compile_options(btrfs PRIVATE "-Werror-implicit-function-declaration")

    target_link_options(btrfs PRIVATE "-nostartfiles")
    target_link_options(btrfs PRIVATE "-shared")
    target_link_options(btrfs PRIVATE "-Wl,--subsystem,11")

    if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "X86")
        target_link_options(btrfs PRIVATE "-e_efi_main")
    else()
        target_link_options(btrfs PRIVATE "-eefi_main")
    endif()
elseif(MSVC)
    target_link_options(btrfs PRIVATE "/SUBSYSTEM:EFI_BOOT_SERVICE_DRIVER")
    target_compile_options(btrfs PRIVATE "/Oi-")
endif()
