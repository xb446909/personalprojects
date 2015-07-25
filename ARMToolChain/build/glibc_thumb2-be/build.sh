#!/bin/bash

PWD=`pwd`
TOOLSDIR=$PWD/../../tools
SRCDIR=$PWD/../../source
SYSDIR=$TOOLSDIR/ToolChain/arm-none-linux-gnueabi/libc/thumb2-be

TARGET=arm-none-linux-gnueabi

CC="$TARGET-gcc -mthumb -march=armv7-a -mbig-endian"    \
CXX="$TARGET-g++ -mthumb -march=armv7-a -mbig-endian"   \
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

PARALLELMFLAGS=-j8 make
make install_root=$SYSDIR install
mkdir $SYSDIR/usr/lib/bin -p
mv $SYSDIR/sbin/ldconfig $SYSDIR/usr/lib/bin
mv $SYSDIR/sbin/sln $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/catchsegv $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/gencat $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/getconf $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/getent $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/iconv $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/ldd $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/locale $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/localedef $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/makedb $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/mtrace $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/pcprofiledump $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/pldd $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/rpcgen $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/sotruss $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/sprof $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/tzselect $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/bin/xtrace $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/sbin/iconvconfig $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/sbin/nscd $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/sbin/zdump $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/sbin/zic $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/libexec/getconf/POSIX_V6_ILP32_OFF32 $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/libexec/getconf/POSIX_V6_ILP32_OFFBIG $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/libexec/getconf/POSIX_V7_ILP32_OFF32 $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/libexec/getconf/POSIX_V7_ILP32_OFFBIG $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/libexec/getconf/XBS5_ILP32_OFF32 $SYSDIR/usr/lib/bin
mv $SYSDIR/usr/libexec/getconf/XBS5_ILP32_OFFBIG $SYSDIR/usr/lib/bin
