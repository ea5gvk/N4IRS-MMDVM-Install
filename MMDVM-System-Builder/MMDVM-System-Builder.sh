#!/usr/bin/env bash
set -o errexit

# N4IRS 06/19/2018

#################################################
#                                               #
#    Build a MMDVM Node on a clean disk         #
#                                               #
#################################################

# Install needed programs
# This could be pruned
#
apt-get update -y
apt-get install git-core -y
apt-get install curl -y
apt-get install g++ -y
apt-get install make -y

# For MMDVM firmware
# apt-get install python-pip -y # Where is this used? 
apt-get install gdb-arm-none-eabi -y
apt-get install libusb-1.0 -y

# For Armbian Need to check this !
# apt-get install libstdc++-arm-none-eabi-newlib -y

cp -rf ./Directories/* /


# Clone the source code for the programs we want to install
# The directories were installed from above

cd /srv/Repositories/CA6JAU
git clone https://github.com/juribeparada/MMDVM_CM.git
git clone https://github.com/juribeparada/MMDVM_HS.git

cd /srv/Repositories/DG9VH
git clone https://github.com/dg9vh/MMDVMHost-Dashboard.git

## cd /srv/Repositories/DVSwitch

cd /srv/Repositories/G4KLX
git clone https://github.com/g4klx/DMRGateway.git
git clone https://github.com/g4klx/P25Clients.git
git clone https://github.com/g4klx/YSFClients.git
git clone https://github.com/g4klx/NXDNClients.git
git clone https://github.com/g4klx/MMDVMHost.git
git clone https://github.com/g4klx/MMDVMCal.git
git clone https://github.com/g4klx/MMDVM.git

cd /srv/Repositories/N0MJS
git clone https://github.com/n0mjs710/dmr_utils.git
git clone https://github.com/n0mjs710/HBlink.git
git clone https://github.com/n0mjs710/DMRlink.git
git clone -b HB_Bridge https://github.com/n0mjs710/HBlink.git HB_Bridge
git clone -b IPSC_Bridge https://github.com/n0mjs710/DMRlink.git IPSC_Bridge

cd /srv/Repositories/stm32flash
git clone https://git.code.sf.net/p/stm32flash/code stm32flash

cd /srv/Repositories/N4IRS
git clone https://github.com/N4IRS/MMDVMVersion.git

# cd /srv/Repositories/OpenDV
# Nothing here

# Copy the source directories to /usr/src
# This allows me to keep a pristine copy in /srv/Repositories

cd /srv/Repositories/CA6JAU/MMDVM_CM
cp -rf DMR2NXDN DMR2YSF NXDN2DMR YSF2DMR YSF2NXDN YSF2P25 /usr/src

cd /srv/Repositories/CA6JAU/
cp -rf MMDVM_HS /usr/src

cd /srv/Repositories/DG9VH
cp -R MMDVMHost-Dashboard/* /var/www/html/

# cd /srv/Repositories/DVSwitch
# Nothing here

cd /srv/Repositories/G4KLX
cp -rf DMRGateway MMDVM MMDVMCal MMDVMHost /usr/src

cd /srv/Repositories/G4KLX/NXDNClients
cp -rf NXDNGateway NXDNParrot /usr/src

cd /srv/Repositories/G4KLX/P25Clients
cp -rf P25Gateway P25Parrot /usr/src

cd /srv/Repositories/G4KLX/YSFClients
cp -rf YSFGateway YSFParrot /usr/src

cd /srv/Repositories/stm32flash
cp -rf stm32flash /usr/src

cd /srv/Repositories/N4IRS
cp -rf MMDVMVersion /usr/src

# cd /srv/Repositories/N0MJS
# Nothing here

## Build the programs from source
## Yes, this is brute force

cd /usr/src/DMR2NXDN
make clean
make
cp DMR2NXDN /opt/DMR2NXDN

cd /usr/src/DMR2YSF
make clean
make
cp DMR2YSF /opt/DMR2YSF

cd /usr/src/DMRGateway
make clean
make
cp DMRGateway /opt/DMRGateway

cd /usr/src/MMDVMCal
make clean
make
cp MMDVMCal /opt/MMDVMCal

cd /usr/src/MMDVMHost
make clean
make
cp MMDVMHost /opt/MMDVMHost

cd /usr/src/NXDN2DMR
make clean
make
cp NXDN2DMR /opt/NXDN2DMR

cd /usr/src/NXDNGateway
make clean
make
cp NXDNGateway /opt/NXDNGateway

cd /usr/src/NXDNParrot
make clean
make
cp NXDNParrot /opt/NXDNParrot

cd /usr/src/P25Gateway
make clean
make
cp P25Gateway /opt/P25Gateway

cd /usr/src/P25Parrot
make clean
make
cp P25Parrot /opt/P25Parrot

cd /usr/src/YSF2DMR
make clean
make
cp YSF2DMR /opt/YSF2DMR

cd /usr/src/YSF2NXDN
make clean
make
cp YSF2NXDN /opt/YSF2NXDN

cd /usr/src/YSF2P25
make clean
make
cp YSF2P25 /opt/YSF2P25

cd /usr/src/YSFGateway
make clean
make
cp YSFGateway /opt/YSFGateway

cd /usr/src/YSFParrot
make clean
make
cp YSFParrot /opt/YSFParrot

cd /usr/src/stm32flash
make clean
make
cp stm32flash /usr/local/sbin

cd /usr/src/MMDVMVersion
make clean
make
cp MMDVMVersion /opt/MMDVMVersion

# This needs to be fleshed out
# MMDVM
# MMDVM_HS


# Enable the systemd unit files
#
systemctl enable nxdngateway.service
systemctl enable nxdnparrot.service
systemctl enable p25gateway.service
systemctl enable p25parrot.service
systemctl enable ysf2dmr.service
systemctl enable ysf2nxdn.service
systemctl enable ysf2p25.service
systemctl enable ysfgateway.service
systemctl enable ysfparrot.service
systemctl enable mmdvmhost.service
systemctl enable netcheck.service

# Populate the datafiles
#
/etc/cron.daily/DMRIDUpdateBM
/etc/cron.daily/FCSRoomsupdate
/etc/cron.daily/HBIDUpdate
/etc/cron.daily/NXDNHostsupdate
/etc/cron.daily/NXDNIDUpdate
/etc/cron.daily/P25Hostsupdate
/etc/cron.daily/TGList-DMR_update
/etc/cron.daily/TGList-NXDN_update
/etc/cron.daily/TGList-P25_update
/etc/cron.daily/XLXHostsupdate
/etc/cron.daily/YSFHostsupdate

# Install the dashboard
#
apt-get install lighttpd -y

# Need to add test for Stretch vs Jessie
# apt-get install php7.0-common -y
# apt-get install php -y
apt-get install php7.0-cgi -y

chown -R www-data:www-data /var/www/html
chmod -R 775 /var/www/html

lighty-enable-mod fastcgi
lighty-enable-mod fastcgi-php

mv /var/www/html/index.lighttpd.html /var/www/html/index.lighttpd.html.old

systemctl restart lighttpd

# Add DVSwitch programs via apt-get install
