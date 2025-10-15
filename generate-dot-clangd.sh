#!/usr/bin/env sh

ZIGPATH=$(which zig)
ZIGPATH=$(dirname -- $ZIGPATH)
ZIGPATH=$(dirname -- $ZIGPATH)
ZIGLIB="$ZIGPATH/lib/zig"

echo -n "CompileFlags:
  Add:
    - \"-I$ZIGLIB/libc/include\"
    - \"-I$ZIGLIB/libcxx/include\"
    - \"-I$ZIGLIB/libcxx/libc/include\"
    - \"-I$ZIGLIB/libcxxabi/include\"
    - \"-std=c17\"
" > ./.clangd
