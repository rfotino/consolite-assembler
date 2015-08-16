CFLAGS = -Wall -Wextra -Werror -std=c++11

all: asm.o tokenizer.o parser.o
	g++ $(CFLAGS) -o asm asm.o tokenizer.o parser.o

asm.o: asm.cpp tokenizer.h parser.h
	g++ $(CFLAGS) -c asm.cpp

tokenizer.o: tokenizer.cpp tokenizer.h defs.h
	g++ $(CFLAGS) -c tokenizer.cpp

parser.o: parser.cpp parser.h tokenizer.h defs.h
	g++ $(CFLAGS) -c parser.cpp

clean:
	rm -f asm *.o *~
