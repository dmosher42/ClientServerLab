all: clean client pipe server

clean:
	rm -rf *.o client pipe server
client:
	gcc client.c -o client
pipe: 
	gcc pipe.c -o pipe
server:
	gcc server.c -o server	


