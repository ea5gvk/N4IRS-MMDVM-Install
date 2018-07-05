# This is the first pass of a script to build svxlink. SVXLink looks like a quite cabable program but I all I'm interested in is devcal (for now)
# This needs to be simplified but for now it works. When the build is complete you will find devcal in /usr/src/svxlink/src/build/bin/devcal

apt-get update
apt-get upgrade -y
# Should reboot here

apt-get update
apt-get install git-core

cd /usr/src
git clone https://github.com/sm0svx/svxlink.git

apt-get install cmake libsigc++-2.0-dev libasound2-dev libpopt-dev libgcrypt11-dev tk-dev libgsm1-dev libspeex-dev libopus-dev groff librtlsdr-dev
cd /usr/src/svxlink/src
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DSYSCONF_INSTALL_DIR=/etc -DLOCAL_STATE_DIR=/var -DUSE_OSS=NO -DUSE_QT=NO ..
make devcal

