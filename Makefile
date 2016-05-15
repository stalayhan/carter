all:
	cd src; make

test-modules:
	cd test_modules; make

clean:
	cd src; make clean

clean-test-modules:
	cd test_modules; make clean
