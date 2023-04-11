sudo apt-get install libasound2-dev libcurl4-openssl-dev cmake libsdl2-dev libfftw3-dev git

# setup Pi-Codec hat
cd ~
git clone https://github.com/iqaudio/Pi-Codec.git
echo "sudo alsactl restore -f ~/Pi-Codec/IQaudIO_Codec_OnboardMIC_record_and_SPK_playback.state" >> ~/.bashrc

# fix clockspeed to allow comms
sudo echo "core_freq=500\ncore_freq_min=500" >> /boot/config.txt

# increase SPI buffer
sudo echo " spidev.bufsiz=32768" >> /boot/cmdline.txt

# build and install libws2811 shared object
git clone https://github.com/jgarff/rpi_ws281x.git
cd ./rpi_ws281x
cmake . -DBUILD_SHARED=ON -DBUILD_TEST=OFF
make
sudo cp ./libws2811.so /usr/local/lib/libws2811.so

# build and install iir1 (infinite impulse response lib) required for envelope following
cd ~
git clone https://github.com/berndporr/iir1.git
cd iir1
cmake . -DIIR1_BUILD_DEMO=OFF -DIIR1_BUILD_TESTING=OFF

echo "Please uncomment the following line in file /boot/config.txt"
echo "#dtparam=audio=on"
echo "Then reboot the Pi"