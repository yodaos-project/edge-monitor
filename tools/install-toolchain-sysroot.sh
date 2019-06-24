#!/bin/bash
set -e
arch_name=$1
export aarch64_toolchain_name="gcc-linaro-5.5.0-2017.10-x86_64_aarch64-linux-gnu"
export aarch64_sysroot_name="sysroot-glibc-linaro-2.21-2017.10-aarch64-linux-gnu"
export armhf_toolchain_name="gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabi"
export armhf_sysroot_name="sysroot-glibc-linaro-2.21-2017.10-arm-linux-gnueabi"

download() {
  aarch=$1
  toolchain_name=$2
  sysroot_name=$3
  if [ ! -d $toolchain_name ] || [ ! -f $toolchain_name/bin/$aarch-gcc ] || [ ! -f $toolchain_name/bin/$aarch-g++ ]; then
    toolchain_url="https://releases.linaro.org/components/toolchain/binaries/latest-5/${aarch}/${toolchain_name}.tar.xz"
    wget $toolchain_url -O ${toolchain_name}.tar.xz
    xz -fd ${toolchain_name}.tar.xz && tar -xf ${toolchain_name}.tar && rm ${toolchain_name}.tar
  else
    echo "cached toolchain $toolchain_name"
  fi
  if [ ! -d $sysroot_name ] || [ ! -d $sysroot_name/lib ] || [ ! -d $sysroot_name/usr ]; then
    sysroot_url="https://releases.linaro.org/components/toolchain/binaries/latest-5/${aarch}/${sysroot_name}.tar.xz"
    wget $sysroot_url -O ${sysroot_name}.tar.xz
    xz -fd ${sysroot_name}.tar.xz && tar -xf ${sysroot_name}.tar && rm ${sysroot_name}.tar
  else
    echo "cached sysroot $sysroot_name"
  fi
}

if [ ! $arch_name ]; then
  download "aarch64-linux-gnu" $aarch64_toolchain_name $aarch64_sysroot_name
  download "arm-linux-gnueabi" $armhf_toolchain_name $armhf_sysroot_name
elif [ "$arch_name" == "aarch64-linux-gnu" ]; then
  download "aarch64-linux-gnu" $aarch64_toolchain_name $aarch64_sysroot_name
elif [ "$arch_name" == "arm-linux-gnueabi" ]; then
  download "arm-linux-gnueabi" $armhf_toolchain_name $armhf_sysroot_name
else
  echo "unkown arch name $arch_name"
  exit 1
fi
