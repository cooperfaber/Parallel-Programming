#	g++ main2.cpp -pthread -O3 -o main2
#g++ main3.cpp -pthread -O3 -o main3
#-fsanitize=thread

all:
	g++ -DSYNCQUEUE main.cpp -pthread -o syncQueue
	g++  main.cpp  -pthread  -o asyncQueue
	g++  main3.cpp -pthread -o queue8
	g++  main4.cpp -pthread -o queue8API

