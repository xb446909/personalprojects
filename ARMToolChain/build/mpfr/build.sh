#!/bin/bash

PWD=`pwd`
TOOLSDIR=$PWD/../../tools
SRCDIR=$PWD/../../source

$SRCDIR/mpfr-3.1.3/configure                \
        --prefix=$TOOLSDIR/mpfr             \
        --with-gmp=$TOOLSDIR/gmp            \
        --enable-static                     \
        --disable-shared

make -j8
make install
