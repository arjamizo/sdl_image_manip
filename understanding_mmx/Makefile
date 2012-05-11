main.o: main.cpp
	g++ -g3 -c main.cpp -o main.o
filter.o: filter.s
	g++ -g3 -c filter.s -o filter.o
main: filter.o main.o
	g++ -g3 filter.o main.o -o main
clean: 
	rm *.o
debug: main 
	gdb ./main