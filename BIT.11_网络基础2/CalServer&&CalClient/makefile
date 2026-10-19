cc=g++
LD=#-DMYSELF

.PHONY:all clean

all:CalClient CalServer

CalClient:cal_client.cc
	$(cc) -o $@ $^ -ljsoncpp $(LD)

CalServer:cal_server.cc
	$(cc) -o $@ $^ -ljsoncpp $(LD)

clean:
	rm -f CalClient CalServer
