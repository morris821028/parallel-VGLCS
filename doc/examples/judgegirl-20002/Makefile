all: main-malloc main-parallel main-static main-jump

main-malloc: main-malloc.cpp
	g++ -std=c++11 -O2 main-malloc.cpp -o main-malloc

main-static: main-static.cpp
	g++ -std=c++11 -O2 main-static.cpp -o main-static

main-parallel: main-parallel.cpp
	g++ -std=c++11 -fopenmp -O2 main-parallel.cpp -o main-parallel

main-jump: main-jump.cpp
	g++ -std=c++11 -fopenmp -O2 main-jump.cpp -o main-jump


clean:
	rm main-malloc main-static main-parallel main-jump
