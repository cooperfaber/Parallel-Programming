
all:
	g++ -DCPP_MUTEX main.cpp -lpthread -o cpp_mutex
	g++ -DFILTER_MUTEX -fsanitize=thread main.cpp -lpthread -o filter_mutex
	g++ -DBAKERY_MUTEX -fsanitize=thread main.cpp -lpthread -o bakery_mutex

clean:
	rm cpp_mutex filter_mutex bakery_mutex
