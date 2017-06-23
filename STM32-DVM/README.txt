Login to Pi-Star via ssh or console
rpi-rw
sudo -s
cd /srv
git clone https://github.com/N4IRS/MMDVM-Install.git
cd /srv/MMDVM-Install/STM32-DVM
./required.sh
./get-src.sh

You now have the needed utilities and source code for the firmware.

cd /usr/src/MMDVM
make -f Makefile.CMSIS

Need to add the notes on putting the board in boot 0 and upload firmware


