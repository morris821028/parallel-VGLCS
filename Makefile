all:
	g++ -O3 -std=c++11 -fopenmp -g3 src/VGLCS-parallel.cpp -o VGLCS-parallel
	#g++ -O3 -std=c++11 -fopenmp VGLCS-layout.cpp -o VGLCS-layout
	#g++ -O2 -std=c++11 VGLCS-nm-parallel.cpp -o VGLCS-parallel-single
	g++ -O3 -std=c++11 src/VGLCS-nm.cpp -o VGLCS
	g++ -O2 -std=c++11 src/pin.cpp -o pin
clean:
	rm VGLCS-parallel
	rm VGLCS
	rm pin
	
