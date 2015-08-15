all: asm.o tokenizer.o parser.o
	g++ -Wall -Wextra -Werror -o asm asm.o tokenizer.o parser.o

asm.o: asm.cpp tokenizer.h parser.h
	g++ -Wall -Wextra -Werror -c asm.cpp

tokenizer.o: tokenizer.cpp tokenizer.h
	g++ -Wall -Wextra -Werror -c tokenizer.cpp

parser.o: parser.cpp parser.h tokenizer.h
	g++ -Wall -Wextra -Werror -c parser.cpp

clean:
	rm -f asm *.o *~
