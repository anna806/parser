
all: parser

parser: parser.o
	g++ -Wall -Wextra -pedantic -std=c++03 parser.o -lwebp -o parser

parser.o: parser.cpp
	g++ -Wall -Wextra -pedantic -std=c++03 -c parser.cpp -o parser

clean:
	rm *.o parser