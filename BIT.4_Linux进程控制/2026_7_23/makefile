.PHONY:all clean
all:main.exe mybin.exe 

main.exe:main.c
	gcc -o $@ $^ -g

mybin.exe:mybin.c
	gcc -o $@ $^ -g

clean:
	rm -f main.exe mybin.exe mycpp
