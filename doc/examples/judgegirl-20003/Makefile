TARGETS= main-serial \
		main-parallel main-parallel-coST

all: $(TARGETS)

main-serial: main-serial.cpp
	g++ -std=c++11 -O2 main-serial.cpp -o main-serial

main-parallel: main-parallel.cpp
	g++ -std=c++11 -fopenmp -O2 main-parallel.cpp -o main-parallel

main-parallel-coST: main-parallel-coST.cpp
	g++ -std=c++11 -fopenmp -O2 main-parallel-coST.cpp -o main-parallel-coST

clean:
	rm $(TARGETS)
