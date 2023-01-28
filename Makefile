all: parse
parse: parse.o
	g++ -O3 parse.o -o parse -lboost_regex -lboost_date_time
parse.o: parse.cpp
	g++ -c parse.cpp
clean:
	rm *.o parse
