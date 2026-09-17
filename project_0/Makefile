.PHONY: clean test

test: p0-bin
	./p0-bin < test.in

p0-bin: Sched.c
	gcc -o p0-bin Sched.c

clean:
	rm -f p0-bin out.txt