I will upload pre-built binaries ready for upload at http://dvswitch.org/files/HAM/MMDVM/

Login to Pi-Star via ssh or console
sudo -s
rpi-rw
pistar-expand
Expand the image to fill the SD card Advanced/Expand
reboot
Login to Pi-Star via ssh or console
rpi-rw
sudo -s
apt-get update
cd /srv
git clone https://github.com/N4IRS/MMDVM-Install.git
cd /srv/MMDVM-Install/STM32-DVM
chmod +x *.sh
./required-libs.sh
./get-src.sh

You now have the needed utilities and source code for the firmware.

cd /usr/src/MMDVM
Do a test to make sure everything builds correctly
make -f Makefile.CMSIS
No errors

For the STM32-DVM USB
Disconnect STM32-DVM from the host
Insert Boot0 jumper
Connect STM32-DVM to the host
PWR, ACT and DMR should be lit solid, NOT flashing.

make -f Makefile.CMSIS program_bl
Disconnect STM32-DVM from host
remove Boot0 jumper
Connect STM32-DVM to host
start MMDVMHost

=================================================

For the STM32-DVM PiHat  
Shutdown the Raspberry Pi
Disconnect power to Raspberry Pi
Insert Boot0 jumper
Power ON the Raspberry Pi
PWR, ACT and DMR should be lit solid, NOT flashing.

make -f Makefile.CMSIS 
program-STM32_DVM_PiHat
Shutdown the Raspberry Pi
Disconnect power to Raspberry Pi
remove Boot0 jumper
Power ON the Raspberry Pi
start MMDVMHost

