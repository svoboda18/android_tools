# Android Tools - A set of binaries
This repo contains a collection of some Android (but not exclusively) tools for various usages and also some libraries (though some are mini-ports) that might be useful.

**Note**: For this project, I worked on it when I was learning, so it is not that perfect and not everything is correct.

## Tools
- e2fsdroid
- ext2simg
- magiskboot
- mke2fs
- e2fstool
- img2simg
- make_ext4fs
- simg2img

## Libraries
- libbase (mini-port)
- libext2_e2p
- libext2_support
- liblzma
- libsparse
- libbz2
- libext2_misc
- libext2_uuid
- libmagiskbase
- libz (old port)
- libcutils (mini-port)
- libext2_profile
- libext2fs
- libmincrypt
- libzopfli
- libext2_blkid
- libext2_quota
- libfdt
- libnt
- libext2_com_err
- libext2_ss
- liblz4
- libselinux

## Build
- Using MSYS2 `clang64` environment with `mingw-w64-clang-x86_64-toolchain` packages group, LLVM version 14 and up, run `mingw32-make` command.
- if built a non-static variant, all DLLs in `out/obj/lib/shared` must be present in your PATH for successful execution. 

## What's changed:
- `e2fsdroid`, `ext2simg` and `mke2fs` had many UBs and most of them are fixed.
- `mke2fs` creates 1k block-sized images without crashing unlike the official release.
- `mke2fs`, `img2simg` and `ext2simg` makes valid sparse images.
- `make_ext4fs` is now using the re-based C++ libsparse.
- All problems with paths should be resolved, both Unix-like and Windows styles work.
- `e2fsdroid` and `make_ext4fs` both work when file config (uid, gid, mode and capability config) or/and selinux contexts are provided.

## For Windows
- There's some UBs/SFs that need to be addressed (test and report).
- Tested and working operations are limited.