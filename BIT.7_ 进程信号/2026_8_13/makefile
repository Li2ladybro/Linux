.PHONY:all clean

all:mytest mysignal

mytest:mytest.cc
	g++ -o $@ $^

mysignal:mysignal.cc
	g++ -o $@ $^

clean:
	rm -f  mytest mysignal