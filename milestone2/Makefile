FLAGS = -g -std=c11 -Werror -lm $(shell pkg-config --cflags --libs check)

all : main.c 
	mkdir -p build
	gcc main.c  -o build/milestone2 $(FLAGS)

clean:
	rm -f *.o 
	rm -fr build/

zip:
	zip milestone2.zip *.c *.h