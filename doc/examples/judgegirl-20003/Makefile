TARGETS= main-serial \
		main-parallel \
		main-parallel-coST \
		main-parallel-coST-part \
		pin

all: $(TARGETS)

pin: pin.cpp
	g++ -std=c++11 -O2 pin.cpp -o pin

main-serial: main-serial.cpp
	g++ -std=c++11 -O2 main-serial.cpp -o main-serial

main-parallel: main-parallel.cpp
	g++ -std=c++11 -fopenmp -O2 main-parallel.cpp -o main-parallel

main-parallel-coST: main-parallel-coST.cpp
	g++ -std=c++11 -fopenmp -O2 main-parallel-coST.cpp -o main-parallel-coST

main-parallel-coST-part: main-parallel-coST-part.cpp
	g++ -std=c++11 -fopenmp -O2 main-parallel-coST-part.cpp -o main-parallel-coST-part

clean:
	rm $(TARGETS)
