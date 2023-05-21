
parser: parser.o
	g++ -m32 parser.o -o parser

parser.o: parser.cpp
	g++ -c parser.cpp -I -L -lwebp

clean:
	rm *.o parser