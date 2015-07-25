#!/bin/bash

PWD=`pwd`
TOOLSDIR=$PWD/../../tools
SRCDIR=$PWD/../../source
SYSDIR=$TOOLSDIR/ToolChain/arm-none-linux-gnueabi/libc

TARGET=arm-none-linux-gnueabi

$SRCDIR/binutils-2.25/configure                 \
        --target=$TARGET                        \
        --prefix=$TOOLSDIR/ToolChain            \
        --with-gmp=$TOOLSDIR/gmp                \
        --with-mpfr=$TOOLSDIR/mpfr              \
        --with-mpc=$TOOLSDIR/mpc                \
        --with-sysroot=$SYSDIR

make -j8
make install
