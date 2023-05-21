
all: parser

parser: parser.o
	g++-7 -Wall -Wextra -pedantic -std=c++17 parser.o -lwebp -o parser

parser.o: parser.cpp
	g++-7 -Wall -Wextra -pedantic -std=c++17 -c parser.cpp -o parser.o

clean:
	rm *.o parser