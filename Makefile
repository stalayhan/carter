all:
	cd lane-detection; make
	cd webcam-test; make
clean:
	cd lane-detection; make clean
	cd webcam-test; make clean
