#!/bin/bash

if [ ! $KAMINO_STAGING_DIR  ]; then
  echo "env \$KAMINO_STAGING_DIR is not defined"
  exit 1
fi
export STAGING_DIR=$KAMINO_STAGING_DIR

ARM_TOOLCHAIN=$STAGING_DIR"/toolchain-arm_cortex-a7+neon_gcc-5.3.0_glibc-2.22_eabi/bin/"
ARM_CC=$STAGING_DIR"/toolchain-arm_cortex-a7+neon_gcc-5.3.0_glibc-2.22_eabi/bin/arm-openwrt-linux-gcc"
ARM_CXX=$STAGING_DIR"/toolchain-arm_cortex-a7+neon_gcc-5.3.0_glibc-2.22_eabi/bin/arm-openwrt-linux-g++"
ARM_SYSROOT=$STAGING_DIR"/toolchain-arm_cortex-a7+neon_gcc-5.3.0_glibc-2.22_eabi/"
ARM_EXT_SYSROOT=$STAGING_DIR"/staging_dir/target-arm_cortex-a7+neon_glibc-2.22_eabi"

cmake \
    -H. \
    -B./build-kamino \
    -DCMAKE_C_COMPILER=$ARM_CC \
    -DCMAKE_CXX_COMPILER=$ARM_CXX \
    -DCMAKE_TOOLCHAIN_ROOT=$ARM_TOOLCHAIN \
    -DCMAKE_EXTERNAL_SYSROOT=$ARM_EXT_SYSROOT \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SYSROOT=$ARM_SYSROOT \
    -DCMAKE_CXX_FLAGS="-O3"

echo "start build"
make -C ./build-kamino


