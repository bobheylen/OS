all:
	mkdir -p build
	gcc -g -Wall -Werror -DSET_MIN_TEMP=15 -DSET_MAX_TEMP=25 main.c datamgr.c lib/dplist.c -o build/datamgr

file_creator:
	gcc -g -Wall -Werror -DDEBUG=ON file_creator.c -o file_creator

clean:
	rm build/*
	rm file_creator