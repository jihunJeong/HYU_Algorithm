all: main

main: main.o project.o
	g++ -o main main.o project.o
main.o: main.cc
	g++ -c main.cc
project.o: project.cc
	g++ -c project.cc

clean:
	rm main *.o
