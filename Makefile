daemon.out : main.c
	gcc -o daemond main.c

clean :
	rm -rf daemond
