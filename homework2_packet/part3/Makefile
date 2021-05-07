
all:
	g++ -DCOARSE_LOCK main.cpp -lpthread -o coarse_lock_stack
	g++ -DRW_LOCK -std=c++17 main.cpp -lpthread -o rw_lock_stack
	g++ -DSWAPTOP -g -std=c++17 main.cpp -lpthread -o swaptop_stack

clean:
	rm coarse_lock_stack rw_lock_stack swaptop_stack
