all:
	gcc -o web-server.out main.c server.c

clean:
	rm -f server
