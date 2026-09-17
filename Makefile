# Name of your compiled executable
TARGET = Sched

# Source and compiler setup
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Test rule for seq0
test: $(TARGET)
	./$(TARGET) seq0.txt
	@echo "Comparing output.txt with seq0.out.txt..."
	diff -u output.txt seq0.out.txt && echo "TEST PASSED!" || echo "TEST FAILED!"

clean:
	rm -f $(TARGET) output.txt