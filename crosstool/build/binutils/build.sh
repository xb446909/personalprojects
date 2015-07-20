#!/bin/bash

PWD=`pwd`
TOOLSDIR=$PWD/../../tools
SRCDIR=$PWD/../../source
SYSDIR=$PWD/../../sysroot

TARGET=arm-none-linux-gnueabi

$SRCDIR/binutils-2.25/configure                 \
        --target=$TARGET                        \
        --prefix=$TOOLSDIR/crosstools           \
        --with-gmp=$TOOLSDIR/gmp                \
        --with-mpfr=$TOOLSDIR/mpfr              \
        --with-mpc=$TOOLSDIR/mpc                \

make -j8
make install
