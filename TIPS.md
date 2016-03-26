Development tips
===

##General

You haven't put the shared library in a location where the loader can find it.
look inside the /usr/local/opencv and /usr/local/opencv2 folders and see if
either of them contains any shared libraries (files beginning in lib and
usually ending in .so). when you find them, create a file called
/etc/ld.so.conf.d/opencv.conf and write to it the paths to the folders where
the libraries are stored, one per line. Then run


```sh
sudo ldconfig -v
```

Configuring CodeBlocks 12.11 IDE for use with OpenCV 2.4.3
---

I have gone through so much pain trying to get CodeBlocks being able to properly use the OpenCV libraries on my system so I decided to write a little something about it. I have Ubuntu 12.04 LTS with CodeBlocks 12.11 (latest stable) and OpenCV 2.4.3 libraries. Follow these steps to setup CodeBlocks IDE to use OpenCV 2.4.3.
Make sure that you have OpenCV installed on your system. There is an excellent guide to follow here that will walk you through the process. You can test your installation by following this guide here.
You can find out where OpenCV and its libraries are installed on your system by using these commands in bash:
```sh
    pkg-config --cflags opencv
    pkg-config --libs opencv 
```
Now you can create a new C++ console project in CodeBlocks.
With the new project, right click on the project in the left bar and choose "Build Options..".
Click on the "Search Directories" tab, then Compiler tab, and add these locations:
    /usr/local/include/opencv
    /usr/local/include/opencv2
Still on the "Search Directories" tab, then click on the the Linker tab and add these locations:
    /usr/local/lib
Then click on the "Linker Settings" tab and be sure to add all of the OpenCV libraries that where shown from step 2. This is a list of all the libraries that I had installed on my system:
    /usr/local/lib/libopencv_calib3d.so
    /usr/local/lib/libopencv_contrib.so
    /usr/local/lib/libopencv_core.so 
    /usr/local/lib/libopencv_features2d.so 
    /usr/local/lib/libopencv_flann.so   
    /usr/local/lib/libopencv_gpu.so 
    /usr/local/lib/libopencv_highgui.so 
    /usr/local/lib/libopencv_imgproc.so 
    /usr/local/lib/libopencv_legacy.so   
    /usr/local/lib/libopencv_ml.so 
    /usr/local/lib/libopencv_nonfree.so 
    /usr/local/lib/libopencv_objdetect.so 
    /usr/local/lib/libopencv_photo.so 
    /usr/local/lib/libopencv_stitching.so 
    /usr/local/lib/libopencv_ts.so 
    /usr/local/lib/libopencv_video.so 
    /usr/local/lib/libopencv_videostab.so
Once you have all that taken care of you can now go ahead and edit the main.cpp source code file. Copy and paste this sample code to test out your setup:

```cpp
#include <stdlib.h>
#include <cv.hpp>
#include <cxcore.hpp>
#include <highgui.h> 

int main(int argc, char** argv)
{
    cv::Mat frame;
    int c;
    CvCapture *capture = cvCaptureFromCAM(0);
    while(1)
    {
        frame = cvQueryFrame(capture);
        cv::imshow("OpenCV",frame);

        c = cv::waitKey(10);
        if(c==27)
            break;
    }
    return 0;
}
```
Done! You can now build and run the sample application to make sure it's working. You should get a new window displaying the image from your webcam. You should now be able to copy the project and use it as a template for future work.

**References:** http://jonniedub.blogspot.com.tr/2013/01/setting-up-codeblocks-ide-for-use-with.html
