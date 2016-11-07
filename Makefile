
CXX=g++

ifeq ($(DEBUG), 1)
CFLAGS=-fPIC -std=c++11 -O2 -fopenmp -g3 -march=native -Wall -Wextra -Wshadow -fno-omit-frame-pointer -fsanitize=address
else
CFLAGS=-fPIC -std=c++11 -O2 -fopenmp -march=native -Wall -Wextra -Wshadow
endif
CFLAGS+= -Iinclude

HEADERS=./include/VGLCS.h ./include/config.h ./include/helper.h
OBJECTS=VGLCS-parallel.o ELVGLCS-parallel.o

all: VG-unit ELVG-unit benchmark pin $(OBJECTS)

VGLCS-parallel.o: ./src/VGLCS-parallel.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c src/VGLCS-parallel.cpp

ELVGLCS-parallel.o: ./src/ELVGLCS-parallel.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c src/ELVGLCS-parallel.cpp

VG-unit: VGLCS-parallel.o ./tests/VG-unit.c $(HEADERS)
	$(CXX) $(CFLAGS) ./tests/VG-unit.c VGLCS-parallel.o -o VG-unit

ELVG-unit: ELVGLCS-parallel.o ./tests/ELVG-unit.c $(HEADERS)
	$(CXX) $(CFLAGS) ./tests/ELVG-unit.c ELVGLCS-parallel.o -o ELVG-unit

pin: ./tests/pin.cpp
	$(CXX) $(CFLAGS) ./tests/pin.cpp -o pin

benchmark: $(OBJECTS) ./benchmarks/benchmark.c $(HEADERS)
	$(CXX) $(CFLAGS) ./benchmarks/benchmark.c VGLCS-parallel.o -o benchmark -lrt

clean:
	-rm VG-unit benchmark pin $(OBJECTS)
	
