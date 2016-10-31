
CXX=g++

ifeq ($(DEBUG), 1)
CFLAGS=-fPIC -std=c++11 -O2 -fopenmp -g3 -march=native -Wall -Wextra -Wshadow -fsanitize=undefined  -fno-omit-frame-pointer -fsanitize=address 
else
CFLAGS=-fPIC -std=c++11 -O2 -fopenmp -march=native -Wall -Wextra -Wshadow
endif
CFLAGS+= -Iinclude

HEADERS=./include/VGLCS.h
OBJECTS=VGLCS-parallel.o

all: unit benchmark $(OBJECTS)

VGLCS-parallel.o: ./src/VGLCS-parallel.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c src/VGLCS-parallel.cpp

unit: VGLCS-parallel.o ./tests/unit.c $(HEADERS)
	$(CXX) $(CFLAGS) ./tests/unit.c VGLCS-parallel.o  -o unit

testdata: ./src/pin.cpp
	$(CXX) $(CFLAGS) src/pin.cpp -o pin

benchmark: VGLCS-parallel.o ./benchmarks/benchmark.c $(HEADERS)
	$(CXX) $(CFLAGS) ./benchmarks/benchmark.c VGLCS-parallel.o -o benchmark -lrt

clean:
	rm unit benchmark VGLCS-parallel.o
	
