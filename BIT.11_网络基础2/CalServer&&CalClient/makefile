cc=g++

.PHONY:all clean

all:CalClient CalServer

CalClient:cal_client.cc
	$(cc) -o $@ $^

CalServer:cal_server.cc
	$(cc) -o $@ $^

clean:
	rm -f CalClient CalServer
