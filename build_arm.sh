#!/bin/bash

platformType=$1
if [ ! $platformType ]; then
  echo 'please specific platform type'
  exit 1
fi

if [ $platformType == 'kamino' ]; then
  if [ ! $KAMINO_STAGING_DIR  ]; then
    echo "env \$KAMINO_STAGING_DIR is not defined"
    exit 1
  fi
  export STAGING_DIR=$KAMINO_STAGING_DIR

  TOOLCHAIN=$STAGING_DIR"/toolchain-arm_cortex-a7+neon_gcc-5.3.0_glibc-2.22_eabi/bin/"
  CC=$TOOLCHAIN"/arm-openwrt-linux-gcc"
  CXX=$TOOLCHAIN"/arm-openwrt-linux-g++"
  SYSROOT=$STAGING_DIR"/toolchain-arm_cortex-a7+neon_gcc-5.3.0_glibc-2.22_eabi/"
  EXT_SYSROOT=$STAGING_DIR"/staging_dir/target-arm_cortex-a7+neon_glibc-2.22_eabi"
elif [ $platformType == 'amlogic'  ]; then
  if [ ! $AMLOGIC_STAGING_DIR  ]; then
    echo "env \$KAMINO_STAGING_DIR is not defined"
    exit 1
  fi
  export STAGING_DIR=$AMLOGIC_STAGING_DIR

  TOOLCHAIN=$STAGING_DIR"/bin"
  CC=$TOOLCHAIN"/aarch64-linux-gnu-gcc"
  CXX=$TOOLCHAIN"/aarch64-linux-gnu-g++"
  SYSROOT=$STAGING_DIR"/aarch64-linux-gnu/libc"
  EXT_SYSROOT=""
else
  echo 'unsupported platform type '$platformType
  exit 1
fi

cmake \
    -H. \
    -B./build-$platformType \
    -DCMAKE_C_COMPILER=$CC \
    -DCMAKE_CXX_COMPILER=$CXX \
    -DCMAKE_TOOLCHAIN_ROOT=$TOOLCHAIN \
    -DCMAKE_EXTERNAL_SYSROOT=$EXT_SYSROOT \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SYSROOT=$SYSROOT \
    -DCMAKE_CXX_FLAGS="-O3"

echo "start build"
make -C ./build-$platformType

