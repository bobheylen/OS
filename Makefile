zip:
	zip milestone3.zip *.c *.h

all: *.c *.h
	mkdir -p build
	gcc -g *.c *.h -o build/main -lpthread -Werror -Wall

run:
	./build/main

clean:
	rm build/*