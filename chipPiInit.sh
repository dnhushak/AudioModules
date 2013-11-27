####################
# Install packages
####################

# Get the git
git clone https://github.com/dnhushak/chipophone

# Install portMedia
sudo apt-get install libasound-dev
sudo apt-get install libportmidi-dev
sudo apt-get install libportmidi0
sudo apt-get install libportaudio2

####################
# TTYMidi Install
####################

wget http://www.varal.org/ttymidi/ttymidi.tar.gz
tar -zxvf ttymidi.tar.gz
cd ttymidi/
make
sudo make install
cd ..
rm -r ttymidi
cd



####################
# Bashrc Edits
####################

sudo echo '# Chipophone Project Commands
# Set the analog out to default (Ignore the HDMI)
amixer -c 0 cset numid=3 1 &> /dev/null

# Enable/disable TTYMidi
alias startmidi="sudo ttymidi -s /dev/ttyAMA0 -b 38400 &"
alias stopmidi="sudo killall ttymidi"

# List MIDI Devices
alias listmidi="aconnect -lio"

# Enable/Disable MIDI throughput
alias midithruon="aconnect 128:0 128:1"
alias midithruoff="aconnect -d 128:0 128:1"
startmidi
midithruon' >> ~/.bashrc



####################
# Baud Rate Trickery
####################

sudo echo '# change uart clock to 2441406 for MIDI 31250 baud rate
# default is init_uart_clock=3000000

init_uart_clock=2441406
init_uart_baud=38400' >> /boot/config.txt

sudo echo 'dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait bcm2708.uart_clock=3000000' > /boot/cmdline.txt

sudo sed -i '/T0:23:respawn:\/sbin\/getty -L ttyAMA0 115200 vt100/ s/^/# /' /etc/inittab