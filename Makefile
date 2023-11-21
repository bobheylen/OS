ex1: main.c
	gcc main.c -o ex1
	./ex1

ex2: main2.c
	gcc main2.c -o ex2
	./ex2

ex3: main3.c
	gcc main3.c -o ex3
	./ex3

clean:
	rm ./ex1 *.o
	rm ./ex2 *.o
	rm ./ex3 *.o
