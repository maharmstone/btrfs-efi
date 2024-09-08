cmake_minimum_required(VERSION 3.14)

project(btrfs-efi LANGUAGES C VERSION 20230328)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

include(GNUInstallDirs)

find_package(EFI REQUIRED)

# -----------------------------------

set(SRC_FILES src/btrfs.c
    src/crc32c.c
    src/misc.c
    src/zlib/adler32.c
    src/zlib/deflate.c
    src/zlib/inffast.c
    src/zlib/inflate.c
    src/zlib/inftrees.c
    src/zlib/trees.c
    src/zlib/zutil.c
    src/lzo.c
    src/xxhash.c
    src/zstd/entropy_common.c
    src/zstd/error_private.c
    src/zstd/fse_decompress.c
    src/zstd/huf_decompress.c
    src/zstd/zstd_common.c
    src/zstd/zstd_ddict.c
    src/zstd/zstd_decompress.c
    src/zstd/zstd_decompress_block.c)

add_library(btrfs SHARED ${SRC_FILES})
target_compile_options(btrfs PRIVATE "-Werror-implicit-function-declaration")

target_link_libraries(btrfs efi)

create_efi_image(btrfs btrfs)

install_efi_image(btrfs ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})
