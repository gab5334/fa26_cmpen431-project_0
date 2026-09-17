.PHONY:clean test

test: p0-bin
	cat test.in | ./p0-bin > out.txt

p0-bin: Sched.c
	gcc -o p0-bin Sched.c

clean:
	-rm p0-bin out.txt