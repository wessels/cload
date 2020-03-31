
cload: cload.c
	gcc -Wall cload.c -o $@ -lncurses -lm

install:
	install -C -m 755 cload /usr/local/bin/cload

clean:
	rm -f cload
