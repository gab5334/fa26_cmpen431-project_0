.PHONY:clean test

test: p0-bin
	cat test.in | ./p0-bin > out.txt

p0-bin: p0-robust.c
	-gcc -o p0-bin p0-robust.c

clean:
	-rm p0-bin out.txt
