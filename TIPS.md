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

