The bossac command is used to upload the MMDVM firmware to the due. On a Debian Jessie x86 platform, you can apt-get install bossa-cli. For other platforms, you will need to build boaasc from source. The repository is at <https://github.com/shumatech/BOSSA> use the arduino branch.
git clone -b arduino https://github.com/shumatech/BOSSA.

If you build the complete package you will need GUI support installed. If all you want is the command line utility, then replace the Makefile with the included one and run:
make bossac
  
bossac will be in the BOSSA/bin directory

