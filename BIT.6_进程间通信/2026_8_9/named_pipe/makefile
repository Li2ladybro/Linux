.PHONY:all
all:server client

server:server.cc
	g++ -o $@ $^ -g
client:client.cc
	g++ -o $@ $^ -g

.PHONY:clean
clean:
	rm -f server client /tmp/namedpipe