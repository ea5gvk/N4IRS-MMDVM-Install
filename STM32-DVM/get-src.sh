#! /bin/bash

cd /srv

wget -O stm32cube_fw_f1_v140.zip http://dvswitch.org/files/HAM/MMDVM/stm32cube_fw_f1_v140.zip

unzip -d /opt stm32cube_fw_f1_v140.zip

cd /srv/
git clone https://github.com/g4klx/MMDVM.git

cp -rf /srv/MMDVM /usr/src

cd /usr/src/MMDVM

git clone https://github.com/juribeparada/STM32F4XX_Lib.git

cp /srv/MMDVM-Install/STM32-DVM/Config.h /usr/src/MMDVM

cp /srv/MMDVM-Install/STM32-DVM/program-STM32_DVM_PiHat /usr/local/sbin

cp /srv/MMDVM-Install/STM32-DVM/program-STM32_DVM_USB /usr/local/sbin



