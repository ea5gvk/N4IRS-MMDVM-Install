#!/usr/bin/env bash
set -o errexit

# N4IRS 127/18/2017

#####################################################
#                                                   #
#      Install mono for DV4Mini or P25NX            #
#                                                   #
#####################################################

apt-get update
apt-get upgrade
apt-get install git dirmngr
apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
echo "deb http://download.mono-project.com/repo/debian stretch main" | sudo tee /etc/apt/sources.list.d/mono-xamarin.list
apt-get update
apt-get install mono-complete mono-devel
