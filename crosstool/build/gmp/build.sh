#!/bin/bash

PWD=`pwd`
TOOLSDIR=$PWD/../../tools
SRCDIR=$PWD/../../source

$SRCDIR/gmp-5.1.3/configure                 \
        --prefix=$TOOLSDIR/gmp              \
        --enable-cxx

make -j8
make install
