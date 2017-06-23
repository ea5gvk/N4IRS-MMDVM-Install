#! /bin/bash

cd /srv

wget -O stm32cube_fw_f1_v140.zip https://sourceforge.net/projects/micro-os-plus/files/Vendor%20Archives/STM32/stm32cube_fw_f1_v140.zip/download

unzip -d /opt stm32cube_fw_f1_v140.zip

cd /srv/
git clone https://github.com/g4klx/MMDVM.git

cp -rf /srv/MMDVM /usr/src

cp Config.h /usr/src/MMDVM





