# DATE_COLLECTION  （AARCH64 & AMD64）

Use To: Collect sensors' data  "include Camera Lidar Radar GPS IMU"
 
Collect sensors' data and pack them in .hdf5 file and use x264 compress the image
At last, use MQTT to send to the server client's topic 
## Getting started
Basic setup in amd
```
sudo apt update 
sudo apt upgrade
sudo apt install cmake  g++
sudo apt install ffmpeg libavcodec-extra libavformat-dev libswscale-dev libavutil-dev
```
Basic setup in arm64(aarch64)
```
sudo apt update 
sudo apt upgrade
sudo apt install cmake  g++  libhdf5-dev
sudo apt install ffmpeg libavcodec-extra libavformat-dev libswscale-dev libavutil-dev
```
Build and run 
```
cd your_workspace/
git clone git@github.com:cowqer/data-collection.git
cd data-collection
mkdir build
cd build
cmake ..
make
./coll
```
## Add to environment    
### !!!please modify " the path to " into the really path of your workspace in own device
```
export LD_LIBRARY_PATH=
/'the/path/to'/data-collection-service/lib/mqtt/amd64: \
/'the/path/to'/data-collection-service/lib/hdf5/amd64: \
/'the/path/to'/data-collection-service/lib/etc: \
/'the/path/to'/data-collection-service/lib/opencv/amd64: \
$LD_LIBRARY_PATH
```
Add relative lib into $LD_LIBRARY_PATH in bashrc 
```
sudo vim ~/.bashrc
```
Add below into the bottom of ".bashrc "
```
export LD_LIBRARY_PATH=/home/cq/data-collection-service/lib/mqtt/amd64:/home/cq/data-collection-service/lib/hdf5/amd64:/home/cq/data-collection-service/lib/etc:/home/cq/data-collection-service/lib/opencv/amd64:/home/cq/data-collection-service/lib/x264/amd64:$LD_LIBRARY_PATH
```
Used to reload a user's Bash configuration file and
update the cache of shared library links
```
sudo source ~/.bashrc
sudo ldconfig

```

## IF ERROR: no found libpthread_nonshared.a(choose one of below commands ):
```
ar cr /usr/lib64/libpthread_nonshared.a
ln -s libpthread.a libpthread_nonshared.a
```
## Cross Compile Command tips
```
arm-linux-gnueabihf-gcc -I/path/to/your/include -L/path/to/your/lib -lyourlibrary your_source.c -o /path/to/your/executable/your_executable_name

arm-linux-gnueabihf-gcc -I./include/mqtt -I./include/openssl -L/home/cq/Desktop/git/data-collection-service/lib/mqtt/arrch64/mqttc/lib -L/home/cq/Desktop/git/data-collection-service/lib/mqtt/arrch64/mqttcpp ./src/pub_0.cpp -o ./arm_test/pub0 -lpaho-mqttpp3 -lpaho-mqtt3as -lpaho-mqtt3c -lssl -lcrypto -lstdc++

arm-linux-gnueabihf-gcc \
  -I./include/mqtt \
  -I./include/openssl \
  -L/home/cq/Desktop/git/data-collection-service/lib/mqtt/arrch64/mqttc/lib \
  -L/home/cq/Desktop/git/data-collection-service/lib/mqtt/arrch64/mqttcpp \
  ./src/pub_0.cpp \
  -o ./arm_test/pub0 \
  -lpaho-mqttpp3 \
  -lpaho-mqtt3as \
  -lpaho-mqtt3c \
  -lssl \
  -lcrypto \
  -lstdc++

./config no-asm shared no-async --prefix=/home/cq/Desktio/arm_openssl --cross-compile-prefix=arm-linux-gnueabihf

sudo chmod 777 client

```
## device show


## Authors 
- [ ] [cowqer](https://github.com/cowqer)
- [ ] [cq's blog](https://www.seekyou.top/)






