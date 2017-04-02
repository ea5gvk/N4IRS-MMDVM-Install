# Support for the GREAT Open Source MMDVM based hotspot
# Build upload-reset
gcc upload-reset.c -o upload-reset

# get the vid and pid of the STMF103
lsusb | grep 1eaf

#run the script
./maple_upload ttyACM0 2 1EAF:0004 zumspot_firmware.bin

N4IRS
