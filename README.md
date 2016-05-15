# carter

Car Computer System - [![Circle CI](https://circleci.com/gh/stalayhan/carter.svg?style=svg)](https://circleci.com/gh/stalayhan/carter) ![Progress](http://progressed.io/bar/75?title=progress)

## Description

Car computer system detects road lane, traffic density and estimated oil consumption using computer vision technologies on embedded device.

This project created for CSE396 - Project II course on GTU.

### Lane Detection

### Speed and Traffic Density Detection

### Estimated Average Oil Consumption

## Dependencies

* OpenCV >= 3.0 - https://github.com/Itseez/opencv/archive/3.0.0.tar.gz
* WiringBP - https://github.com/zhaolei/WiringOP

This project tested and verified on Linux i3 4.2.0-36-generic #41~14.04.1-Ubuntu system. If you 

## Install a checkout from git

```sh
git clone https://github.com/stalayhan/carter   # clone project from git repo
cd carter                                       # enter project dir
make                                            # make src - if you want to build modules as separate use make test-modules
./main -v <Video Path>                          # if you do not use -v option, default using first webcam on the system.

```

## Hardware

* Vibration Motor - 5V
* OrangePi Mini PC 2 - 1.6 Ghz Quad Core - 1 Gb Ram - Armbian x64
* 10K Resistor and Transistor for controlling vibration motor
* 5V DC Fan - (5000 RPM) for controlling temperature

## TO-DO and Future Works

* Integration speed detection using frame difference using lane detection
* Add virtual lane if system does not detect any.
* Add traffic sign detection using red and blue colors density without machine learning.

## Pull Request as always welcome

* Fork the repo and create your branch from dev.
* If you've changed codes, update the documentation.
* Make sure your code lints.
* Change README.md if necessary

## Project Members

* Furkan Aksoy
* Hazel Pala
* Eyüp Şen
* Yavuz Şen
* Mahmut Ensari Güvengil
* Oğuzcan Pamuk
* Samet Talayhan - @talayhans

## LICENSE

The MIT License (MIT)
Copyright © 2016 GTU

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
