
CXX=g++

ifeq ($(DEBUG), 1)
CFLAGS=-fPIC -std=c++11 -O2 -fopenmp -g3 -march=native -Wall -Wextra -Wshadow -fsanitize=undefined  -fno-omit-frame-pointer -fsanitize=address 
else
CFLAGS=-fPIC -std=c++11 -O2 -fopenmp -march=native -Wall -Wextra -Wshadow
endif
CFLAGS+= -Iinclude

all: unit

VGLCS-parallel.o: ./src/VGLCS-parallel.cpp ./src/VGLCS-dev.cpp ./src/VGLCS-nm.cpp
	$(CXX) $(CFLAGS) -c src/VGLCS-parallel.cpp

unit: VGLCS-parallel.o ./tests/unit.c
	$(CXX) $(CFLAGS) ./tests/unit.c VGLCS-parallel.o  -o unit

testdata: src/pin.cpp
	$(CXX) $(CFLAGS) src/pin.cpp -o pin

clean:
	rm VGLCS-parallel VGLCS pin
	
