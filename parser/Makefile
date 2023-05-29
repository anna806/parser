
all: parser

parser: parser.o parse_caff.o parse_ciff.o
	g++ -Wall -Wextra -pedantic -std=c++20 parser.o parse_caff.o parse_ciff.o -lwebp -o parser

parser.o: parser.cpp
	g++ -Wall -Wextra -pedantic -std=c++20 -c parser.cpp -o parser.o

parse_caff.o: parser_caff.cpp parse_caff.h
	g++ -Wall -Wextra -pedantic -std=c++20 -c parse_caff.cpp -o parse_caff.o

parse_ciff.o: parse_ciff.cpp parse_ciff.h
	g++ -Wall -Wextra -pedantic -std=c++20 -c parse_ciff.cpp -o parse_ciff.o

clean:
	rm *.o parser