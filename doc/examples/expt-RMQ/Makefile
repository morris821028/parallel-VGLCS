all: rmq cormq

rmq: rmq.cpp
	g++ -std=c++11 -O2 -fopenmp rmq.cpp -o rmq
cormq: cormq.cpp
	g++ -std=c++11 -O2 -fopenmp cormq.cpp -o cormq

clean:
	rm rmq cormq
