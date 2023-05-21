
all: parser

parser: parser.o
	g++ -Wall -Wextra -pedantic -std=c++20 parser.o -lwebp -o parser

parser.o: parser.cpp
	g++ -Wall -Wextra -pedantic -std=c++20 -c parser.cpp -o parser.o

clean:
	rm *.o parser