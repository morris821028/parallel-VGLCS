.PHONY: all

all: main-parallel main-serial

main-parallel: main-parallel.cpp
	g++ -std=c++11 -O2 -fopenmp main-parallel.cpp -o main-parallel

main-serial: main-serial.cpp
	g++ -std=c++11 -O2 main-serial.cpp -o main-serial

clean:
	rm main-serial main-parallel
