test_server: test_server.c lib/tcpsock.c
	gcc -g test_server.c lib/tcpsock.c -o test_server

sensor_node: sensor_node.c lib/tcpsock.c
	gcc sensor_node.c lib/tcpsock.c -DLOOPS=6 -o sensor_node


all: sensor_node test_server
	@echo done

run: 
	./test_server 1234 &
	sleep 1
	./sensor_node 1111 1 127.0.0.1 1234 &
	./sensor_node 2222 1 127.0.0.1 1234 &
	./sensor_node 3333 1 127.0.0.1 1234 &

clean:
	rm test_server sensor_node
