
* Private Gist: [Private Gist](https://gist.github.com/stalayhan/15375ed9b7c59007b124b608ec04c38a)

```sh
sudo apt-get install libopencv-dev python-opencv
sudo apt-get -y install libopencv-dev build-essential cmake git
sudo apt-get -y install libgtk2.0-dev pkg-config python-dev python-numpy libdc1394-22
sudo apt-get -y install libdc1394-22-dev libjpeg-dev libpng12-dev libtiff4-dev
sudo apt-get -y install libjasper-dev libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get -y install  libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev
sudo apt-get -y install libv4l-dev libqt4-dev libmp3lame-dev libopencore-amrnb-dev
sudo apt-get -y install libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 
sudo apt-get -y install v4l-utils unzip

```
COMPILE AND INSTALL OPENCV

In your terminal, make sure you are within the OpenCV directory and run the following commands:

```sh
mkdir build
cd build
cmake -DWITH_OPENGL=ON -DWITH_TBB=ON -DBUILD_EXAMPLES=ON ..
make -j4
sudo make install
sudo ldconfig
```

PS: I have got some errors trying to install below packages on rapsbian. Then some googling I decided to install manually from source code happily there is no problem to compile and installation time but when using VideoCapture class with video file it's not working properly. 
