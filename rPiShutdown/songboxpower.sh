echo '#!/bin/bash

#this is the GPIO pin connected to the lead on switch labeled OUT
GPIOpin1=23

#this is the GPIO pin connected to the lead on switch labeled IN
GPIOpin2=24

#this is the GPIO pin connected to the lead on switch labeled IN
GPIOpin3=25

echo "$GPIOpin1" > /sys/class/gpio/export
echo "in" > /sys/class/gpio/gpio$GPIOpin1/direction
echo "$GPIOpin2" > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio$GPIOpin2/direction
echo "1" > /sys/class/gpio/gpio$GPIOpin2/value
echo "$GPIOpin3" > /sys/class/gpio/export
echo "in" > /sys/class/gpio/gpio$GPIOpin1/direction
while [ 1 = 1 ]; do
power=$(cat /sys/class/gpio/gpio$GPIOpin1/value)
otherpi=$(cat /sys/class/gpio/gpio$GPIOpin3/value)
if [ $power = 1 ] & [ $otherpi = 0 ]; then
echo "0" > /sys/class/gpio/gpio$GPIOpin2/value
sudo poweroff
else
sleep 1
fi
done' > /etc/switch.sh
sudo chmod 777 /etc/switch.sh
sudo sed -i '$ i /etc/switch.sh &' /etc/rc.local
