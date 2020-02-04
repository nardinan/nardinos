#!/bin/bash
brew install gcc gmp mpfr libmpc
# Preparing the required environmental variables
export PREFIX="/usr/local/i386_elf_gcc"
sudo mkdir -p $PREFIX
sudo chown $USER $PREFIX
export TARGET=i386-elf
export CXX=g++
export CC=gcc
export LD=gcc
#export CXX=/usr/local/bin/g++-9
#export CC=/usr/local/bin/gcc-9
#export LD=/usr/local/bin/gcc-9
export CFLAGS=-Wno-error=deprecated-declarations
export PATH="$PREFIX/bin:$PATH"
# OK now we need to start collecting the different tools we need, starting from binutils
# (the latest version of binutils is 2.34 now)
mkdir -p /tmp/src
cd /tmp/src
if [ ! -d "/tmp/src/binutils-2.34" ]; then
  curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.34.tar.gz
  tar xvzf binutils-2.34.tar.gz 
fi
if [ ! -d "/tmp/src/binutils-build" ]; then
  mkdir -p binutils-build
  cd binutils-build
  ../binutils-2.34/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
  make all install 2>&1 | tee make.log
fi
# And now we should use gcc to prepare the different elements (the latest version of gcc is 9.2.0 now)
cd /tmp/src
if  [ ! -d "/tmp/src/gcc-9.2.0" ]; then
  curl -O http://ftp.gnu.org/gnu/gcc/gcc-9.2.0/gcc-9.2.0.tar.gz
  tar xvzf gcc-9.2.0.tar.gz
fi
if [ ! -d "/tmp/src/gcc-build" ]; then
  mkdir -p gcc-build
  cd gcc-build
  ../gcc-9.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c --without-headers \
    --with-mpft=/usr/local --with-gmp=/usr/local --with-mpc=/usr/local
  make all-gcc 
  make all-target-libgcc 
  make install-gcc 
  make install-target-libgcc
fi
# And, to complete, we're going to install gdb that we'll use to debug our kernel
cd /tmp/src
if [ ! -d "/tmp/src/gdb-8.3" ]; then
  curl -O http://ftp.rediris.es/mirror/GNU/gdb/gdb-8.3.tar.gz
  tar xf gdb-8.3.tar.gz
fi
if [ ! -d "/tmp/src/gdb-build" ]; then
  mkdir gdb-build
  cd gdb-build
  ../gdb-8.3/configure --target="$TARGET" --prefix="$PREFIX" --program-prefix=i386-elf-
  make
  make install
fi
