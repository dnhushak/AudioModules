####################
# Install packages
####################

# Get the git
git clone https://github.com/dnhushak/chipophone

# Install portMedia
sudo apt-get uninstall remove jackd1 jackd2
sudo apt-get install libasound2-dev libportmidi-dev libportmidi0
wget http://www.portaudio.com/archives/pa_stable_v19_20111121.tgz
tar -xvf pa_stable_v19_20111121.tgz
cd portaudio
./configure
make
sudo make install
cd ..
rm -r portaudio
rm pa_stable_v19_20111121.tgz
sudo mv /usr/local/lib/libportaudio.* /usr/lib/

#wget http://downloads.sourceforge.net/project/portmedia/portmidi/217/portmidi-src-217.zip
#unzip portmidi-src-217.zip

####################
# ALSA Setup
####################
sudo echo '
pcm.mmap0 {
    type mmap_emul;
    slave {
      pcm "hw:0,0";
    }
}

pcm.!default {
  type plug;
  slave {
    pcm mmap0;
  }
}

ctl.!default {
        type hw
        card 0
}
' >> /etc/asound.conf

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
cd ~/

####################
# Baud Rate Trickery
####################

# Underclock UART
sudo echo '
# change uart clock to 2441406 for MIDI 31250 baud rate
# default is init_uart_clock=3000000

init_uart_clock=2441406
init_uart_baud=38400' >> /boot/config.txt

sudo echo 'dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait bcm2708.uart_clock=3000000' > /boot/cmdline.txt

# Disable GETTY on 
sudo sed -i '/T0:23:respawn:\/sbin\/getty -L ttyAMA0 115200 vt100/ s/^/# /' /etc/inittab > /dev/null



####################
# Bashrc Edits
####################

sudo echo '

# Chipophone Project Commands
# Set the analog out to default (Ignore the HDMI)
amixer -c 0 cset numid=3 1 &> /dev/null

# Enable/disable TTYMidi
alias startmidi="sudo ttymidi -s /dev/ttyAMA0 -b 38400 &"
alias stopmidi="sudo killall ttymidi"

alias testaudio="sudo aplay /usr/share/sounds/alsa/Front_Center.wav"

# List MIDI Devices
alias listmidi="aconnect -lio"

# Enable/Disable MIDI throughput
alias midithruon="aconnect 128:0 128:1"
alias midithruoff="aconnect -d 128:0 128:1"
startmidi
sleep 2
midithruon' >> ~/.bashrc

####################
# ALSA Errors - comment out unused "default" cards
####################

sudo sed -i '/pcm.front cards.pcm.front/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.rear cards.pcm.rear/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.center_lfe cards.pcm.center_lfe/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.side cards.pcm.side/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.surround40 cards.pcm.surround40/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.surround41 cards.pcm.surround41/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.surround50 cards.pcm.surround50/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.surround51 cards.pcm.surround51/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.surround71 cards.pcm.surround71/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.iec958 cards.pcm.iec958/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.spdif iec958/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.hdmi cards.pcm.hdmi/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.dmix cards.pcm.dmix/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.dsnoop cards.pcm.dsnoop/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.modem cards.pcm.modem/ s/^/# /' /usr/share/alsa/alsa.conf
sudo sed -i '/pcm.phoneline cards.pcm.phoneline/ s/^/# /' /usr/share/alsa/alsa.conf
