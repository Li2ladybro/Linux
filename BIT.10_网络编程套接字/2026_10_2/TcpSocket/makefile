cc=g++

.PHONY:all clean

all:TcpClient TcpServer

TcpClient:TcpClient.cc
	$(cc) -o $@ $^

TcpServer:TcpServer.cc
	$(cc) -o $@ $^

clean:
	rm -f TcpServer TcpClient
