#!/bin/bash

ls -lh

PROJ_DIR=`echo $(cd $(dirname "$0")/../; pwd)`

while [ $# -gt 0 ]; do
  case "$1" in
    --host)
      HOST=$2
      shift
      ;;
    --sysroot)
      SYSROOT=$2
      shift
      ;;
    --toolchain)
      TOOLCHAIN=$2
      shift
      ;;
    -s)
      STRIP_EXEC="true"
      ;;
    -j)
      PAR=$2
      shift
      ;;
    -h)
      printf "$help"
      exit
      ;;
    --*)
      echo "Illegal option $1"
      exit
      ;;
  esac
  shift $(( $# > 0 ? 1 : 0 ))
done

if [ ! $TOOLCHAIN ]; then
  CC=`echo $(which gcc)`
  CXX=`echo $(which g++)`
  STRIP=`echo $(which strip)`
  if [ ! $HOST ]; then
    HOST=`echo $(uname -m)`
  fi
else
  TOOLCHAIN=`echo "$(cd ${TOOLCHAIN}; pwd)"`
  if [ ! $HOST ]; then
    echo "Please define host for toolchain."
    exit 1
  fi
  CC=$TOOLCHAIN/$HOST-gcc
  CXX=$TOOLCHAIN/$HOST-g++
  STRIP=$TOOLCHAIN/$HOST-strip
fi

if [ ! $SYSROOT ]; then
  SYSROOT=/
else
  SYSROOT=`echo "$(cd ${SYSROOT}; pwd)"`
fi

if [ ! $PAR ]; then
  PAR=1
fi

BUILD_DIR=`pwd`/build-$HOST

cmake \
  -H$PROJ_DIR \
  -B$BUILD_DIR \
  -DCMAKE_C_COMPILER=$CC \
  -DCMAKE_CXX_COMPILER=$CXX \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_SYSROOT=$SYSROOT \
  -DCMAKE_CXX_FLAGS="-O3" \
&& \
make -C ./build-$HOST -j$PAR
if [ $STRIP_EXEC ]; then
  echo "Stripe executable $BUILD_DIR/yoda-sixsix."
  $STRIP $BUILD_DIR/yoda-sixsix
fi
