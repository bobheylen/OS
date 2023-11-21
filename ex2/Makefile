FLAGS = -g -std=c11 -Werror -lm $(shell pkg-config --cflags --libs check)

all : main.c 
	mkdir -p build
	gcc main.c  -o build/demoFork $(FLAGS)

clean:
	rm -f *.o 
	rm -fr build/
