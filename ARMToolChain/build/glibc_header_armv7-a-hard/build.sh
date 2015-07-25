#!/bin/bash

PWD=`pwd`
TOOLSDIR=$PWD/../../tools
SRCDIR=$PWD/../../source
SYSDIR=$TOOLSDIR/ToolChain/arm-none-linux-gnueabi/libc/armv7-a-hard
mkdir -p $SYSDIR

TARGET=arm-none-linux-gnueabi

CC="$TARGET-gcc -march=armv7-a -mfloat-abi=hard -mfpu=neon"     \
CXX="$TARGET-g++ -march=armv7-a -mfloat-abi=hard -mfpu=neon"    \
CFLAGS="-g -O2"                                 \
AR=$TARGET-ar                                   \
NM=$TARGET-nm                                   \
RANLIB=$TARGET-ranlib                           \
READELF=$TARGET-readelf                         \
$SRCDIR/glibc-2.21/configure                    \
       --host=$TARGET                           \
       --prefix=/usr                            \
       --with-headers=$SYSDIR/../usr/include    \
       --disable-profile                        \
       --enable-add-ons                         \
       --enable-kernel=2.6.32                   \
       --enable-obsolete-rpc                    \
       libc_cv_forced_unwind=yes                \
       libc_cv_ctros_header=yes                 \
       libc_cv_c_cleanup=yes

make install_root=$SYSDIR install-bootstrap-headers=yes install-headers
make csu/subdir_lib
mkdir $SYSDIR/usr/lib -p
cp csu/crt1.o csu/crti.o csu/crtn.o $SYSDIR/usr/lib/
$TARGET-gcc -march=armv4t -o $SYSDIR/usr/lib/libc.so -nostdlib -nostartfiles -shared -x c /dev/null

