CXX=g++
LIBS=`pkg-config --libs opencv`
INC=`pkg-config --cflags opencv`
FLAGS=-g -Wall

main:main.o lane_detector.o
	$(CXX) -o $@ $^ $(LIBS) $(FLAGS)
main.o:main.cpp
	$(CXX) $^ -c -o $@ $(INC) $(FLAGS)
lane_detector.o:lane_detector.cpp
	$(CXX) $^ -c -o $@ $(INC) $(FLAGS)

clean:
	rm main *.o
