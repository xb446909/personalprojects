#!/bin/bash

PWD=`pwd`
TOOLSDIR=$PWD/../../tools
SRCDIR=$PWD/../../source
SYSDIR=$TOOLSDIR/crosstools/arm-none-linux-gnueabi/libc

TARGET=arm-none-linux-gnueabi

$SRCDIR/gcc-5.2.0/configure                     \
        --target=$TARGET                        \
        --prefix=$TOOLSDIR/crosstools           \
        --with-gmp=$TOOLSDIR/gmp                \
        --with-mpfr=$TOOLSDIR/mpfr              \
        --with-mpc=$TOOLSDIR/mpc                \
        --with-sysroot=$SYSDIR                  \
        --with-build-time-tools=$SYSDIR/../bin  \
        --with-newlib                           \
        --without-headers                       \
        --enable-bootstrap                      \
        --disable-libmudflap                    \
        --disable-shared                        \
        --disable-threads                       \
        --disable-decimal-float                 \
        --disable-libssp                        \
        --disable-libstdcxx-pch                 \
        --enable-extra-sgxxlite-multilibs       \
        --with-arch=armv7-a                     \
        --with-gnu-ld                           \
        --with-gnu-as                           \
        --enable-languages=c


make all-gcc -j8
make install-gcc
make all-target-libgcc -j8
make install-target-libgcc
