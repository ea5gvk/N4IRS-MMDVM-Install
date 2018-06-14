#!/usr/bin/env bash
# set -o errexit

# N4IRS 07/26/2017

#################################################
#                                               #
#                                               #
#                                               #
#################################################


apt-get install g++ -y
apt-get install make -y
apt-get install build-essential -y

apt-get install git -y
# apt-get install dkms -y
apt-get install debhelper -y
apt-get install pkg-config -y

apt-get install cmake -y
apt-get install libtool -y

# For MD380-emu

apt-get install gcc-arm-none-eabi -y
apt-get install binutils-arm-none-eabi -y
apt-get install libnewlib-arm-none-eabi -y

apt-get install libc6-armel-cross -y
apt-get install libc6-dev-armel-cross -y
apt-get install binutils-arm-linux-gnueabi -y

apt-get install gcc-arm-linux-gnueabihf -y
apt-get install g++-arm-linux-gnueabihf -y

apt-get install qemu binfmt-support -y
apt-get install qemu-user-static -y

# For ircDDBGateway

apt-get install automake -y
# apt-get install libwxgtk3.0-dev -y
apt-get install fakeroot -y
