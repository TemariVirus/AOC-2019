#!/usr/bin/env sh

ZIGLIB=$(zig env | grep lib_dir | cut -d'"' -f2)

echo -n "CompileFlags:
  Add:
    - \"-I$ZIGLIB/include\"
    - \"-I$ZIGLIB/libc/glibc/include\"
    - \"-I$ZIGLIB/libc/include\"
    - \"-I$ZIGLIB/libc/include/generic-glibc/\"
    - \"-std=gnu17\"
" > ./.clangd
