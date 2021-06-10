#-fsanitize=thread

all:
	g++ -std=c++17 main1.cpp -fsanitize=thread -lpthread -o sjbarrier
	g++ -std=c++17 -DSRBARRIER main2.cpp -lpthread -o srbarrier
	g++ -std=c++17 -DSROBARRIER main2.cpp -lpthread -o srobarrier
