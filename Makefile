
parser: parser.o
	g++ parser.o -o parser

parser.o: parser.cpp encode.h
	g++ -c parser.cpp -I -L -lwebp

clean:
	rm *.o parser