all: server client
server: server.c
	gcc server.c -o server -lsocket -lnsl -Wall
client: client.c
	gcc client.c -o client -lsocket -lnsl -Wall
