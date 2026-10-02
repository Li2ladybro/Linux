cc=g++

.PHONY:all clean

all:udpClient udpServer

udpClient:udpClient.cc
	$(cc) -o $@ $^

udpServer:udpServer.cc
	$(cc) -o $@ $^

clean:
	rm -f udpServer udpClient
