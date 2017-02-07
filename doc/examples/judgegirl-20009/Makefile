TARGET=ISMQ ISMQ-disjoint ISMQ-ST ISMQ-coST ISMQ-BIT

CFLAG=-Wall -std=c++11 -O2 -msse -march=native
CXX=g++

all: $(TARGET)

ISMQ: main.cpp ISMQ.cpp
	$(CXX) $(CFLAG) main.cpp ISMQ.cpp -o ISMQ

ISMQ-disjoint: main.cpp ISMQ-disjoint.cpp
	$(CXX) $(CFLAG) main.cpp ISMQ-disjoint.cpp -o ISMQ-disjoint

ISMQ-ST: main.cpp ISMQ-ST.cpp
	$(CXX) $(CFLAG) main.cpp ISMQ-ST.cpp -o ISMQ-ST

ISMQ-coST: main.cpp ISMQ-coST.cpp
	$(CXX) $(CFLAG) main.cpp ISMQ-coST.cpp -o ISMQ-coST

ISMQ-BIT: main.cpp ISMQ-BIT.cpp
	$(CXX) $(CFLAG) main.cpp ISMQ-BIT.cpp -o ISMQ-BIT

TIN=7.in

test:
	time -p ./ISMQ-disjoint <$(TIN) >/dev/null
	time -p ./ISMQ-ST <$(TIN) >/dev/null
	time -p ./ISMQ-coST <$(TIN) >/dev/null
	time -p ./ISMQ-BIT <$(TIN) >/dev/null

clean:
	rm $(TARGET)
