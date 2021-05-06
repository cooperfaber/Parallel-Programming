
all:
	g++ -DCOARSE_LOCK -fsanitize=thread main.cpp -lpthread -o coarse_lock_stack
	g++ -DRW_LOCK -fsanitize=thread -std=c++17 main.cpp -lpthread -o rw_lock_stack
	g++ -DSWAPTOP -fsanitize=thread -g -std=c++17 main.cpp -lpthread -o swaptop_stack

clean:
	rm coarse_lock_stack rw_lock_stack swaptop_stack
