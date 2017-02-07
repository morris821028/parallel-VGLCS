TARGET=ISMQ ISMQ-disjoint ISMQ-ST ISMQ-coST ISMQ-BIT ISMQ-oneST

CFLAG=-Wall -std=c++11 -O2 -msse -march=native
CXX=g++

all: $(TARGET)

main.o: main2.cpp
	$(CXX) $(CFLAG) -c main2.cpp -o main.o

ISMQ: main.o ISMQ.cpp
	$(CXX) $(CFLAG) main.o ISMQ.cpp -o ISMQ

ISMQ-disjoint: main.o ISMQ-disjoint.cpp
	$(CXX) $(CFLAG) main.o ISMQ-disjoint.cpp -o ISMQ-disjoint

ISMQ-ST: main.o ISMQ-ST.cpp
	$(CXX) $(CFLAG) main.o ISMQ-ST.cpp -o ISMQ-ST

ISMQ-coST: main.o ISMQ-coST.cpp
	$(CXX) $(CFLAG) main.o ISMQ-coST.cpp -o ISMQ-coST

ISMQ-oneST: main.o ISMQ-oneST.cpp
	$(CXX) $(CFLAG) main.o ISMQ-oneST.cpp -o ISMQ-oneST

ISMQ-BIT: main.o ISMQ-BIT.cpp
	$(CXX) $(CFLAG) main.o ISMQ-BIT.cpp -o ISMQ-BIT

TIN=7.in

test:
	time -p ./ISMQ-disjoint <$(TIN) >/dev/null
	time -p ./ISMQ-ST <$(TIN) >/dev/null
	time -p ./ISMQ-coST <$(TIN) >/dev/null
	time -p ./ISMQ-BIT <$(TIN) >/dev/null

clean:
	rm $(TARGET)
