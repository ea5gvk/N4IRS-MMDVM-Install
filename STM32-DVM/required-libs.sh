#! /bin/sh

#####################################################
#                                                   #
#    Install libs required to build and install     #
#            for the STM32-DVM                      #
#                                                   #
#####################################################

apt-get install -y gcc-arm-none-eabi
apt-get install -y binutils-arm-none-eabi
apt-get install -y libusb-1.0
apt-get install -y libnewlib-arm-none-eabi
apt-get install -y make
apt-get install -y curl
apt-get install -y python-pip

apt-get install stm32flash

exit 0

apt-get install libusb-dev -y
apt-get install libnewt-dev -y
apt-get install libeditline0 -y
apt-get install libncurses5-dev -y

apt-get install bison -y

apt-get install libssl-dev -y

apt-get install libasound2-dev -y

apt-get install libcurl4-gnutls-dev -y

apt-get install php5-cli -y

apt-get install libiksemel-dev -y

apt-get install libvorbis-dev -y

apt-get install curl -y

# Nice to have utilities and tools
# is sox required or nice to have
apt-get install sox -y
apt-get install usbutils -y
apt-get install alsa-utils -y
apt-get install bc -y
apt-get install dnsutils -y

apt-get install g++ -y
apt-get install make -y
apt-get install build-essential -y

apt-get install git -y

