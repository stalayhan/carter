CXX=g++
LIBS=`pkg-config --libs opencv`
INC=`pkg-config --cflags opencv`
FLAGS=-g -Wall

main:main.o 
	$(CXX) -o $@ $^ $(LIBS) $(FLAGS)
main.o:main.cpp
	$(CXX) $^ -c -o $@ $(INC) $(FLAGS)
clean:
	rm main *.o
