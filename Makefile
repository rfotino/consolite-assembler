CFLAGS = -Wall -Wextra -Werror -std=c++11

all: asm.o tokenizer.o parser.o
	g++ $(CFLAGS) -o asm bin/asm.o bin/tokenizer.o bin/parser.o

asm.o: src/asm.cpp src/tokenizer.h src/parser.h
	g++ $(CFLAGS) -o bin/asm.o -c src/asm.cpp

tokenizer.o: src/tokenizer.cpp src/tokenizer.h src/defs.h
	g++ $(CFLAGS) -o bin/tokenizer.o -c src/tokenizer.cpp

parser.o: src/parser.cpp src/parser.h src/tokenizer.h src/defs.h
	g++ $(CFLAGS) -o bin/parser.o -c src/parser.cpp

clean:
	rm -f asm bin/*.o src/*~
