
TARGET=rmq \
	cormq rmq-serial\
	demo

all: ${TARGET}

demo: demo.cpp
	g++ -std=c++11 -O2 demo.cpp -o demo

rmq: rmq.cpp
	g++ -std=c++11 -O2 -fopenmp rmq.cpp -o rmq

cormq: cormq.cpp
	g++ -std=c++11 -O2 -fopenmp cormq.cpp -o cormq

rmq-serial: rmq-serial.cpp
	g++ -std=c++11 -O2 -fopenmp rmq-serial.cpp -o rmq-serial

clean:
	rm ${TARGET}
