# devcal
This is the devcal program from SM0SVX for use on SvxLink 
It has been modified (slightly) to maker it easier to use with MMDVMCal
\
devcal v1.0.1.99.0 Copyright (C) 2003-2015 Tobias Blomberg / SM0SVX

devcal comes with ABSOLUTELY NO WARRANTY. This is free software, and you
are welcome to redistribute it in accordance with the terms and conditions in
the GNU GPL (General Public License) version 2 or later.
Minor tweaks by N4IRS for use with MMDVMCal.

Usage: devcal <config file> <config section>
  -f, --modfqs=<frequences in Hz>     The frequencies of the sine waves to modulate with (default: "1000.0")  
  -z, --rxfreq=<frequency in Hz>      The frequency of the receiver (default: "433000000")  
  -d, --caldev=<deviation in Hz>      The deviation to calibrate with (default: 2404.8)  
  -m, --maxdev=<deviation in Hz>      The maximum deviation for the channel (default: 5000)  
  -H, --headroom=<headroom in dB>     The headroom to use (default: 6)  
  -r, --rxcal                         Do receiver calibration  
  -t, --txcal                         Do transmitter calibration  
  -F, --flat                          Flat TX/RX frequency response (no emphasis)  
  -M, --measure                       Measure deviation  
  -w, --wide                          Wideband mode  
  -a, --audiodev=<type:dev>           The audio device to use for audio output (default: "alsa:default")
Help options:
  -?, --help                          Show this help message  
      --usage                         Display brief usage message
      
./devcal --measure --modfqs 1200 --rxfreq 441000000 ./devcal.conf DEVCAL

devcal.conf is the configuration file and DEVCAL is the stanza to use

