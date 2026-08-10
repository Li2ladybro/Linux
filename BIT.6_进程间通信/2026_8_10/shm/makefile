.PHONY:all clean
all:shmClient shmServer

shmClient:shmClient.cc
	g++ -o $@ $^
shmServer:shmServer.cc
	g++ -o $@ $^

clean:
	rm -f shmClient shmServer

