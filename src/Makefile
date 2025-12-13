CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_GNU_SOURCE -Wno-sign-compare
TARGET = minidocker
SOURCES = main.c container.c filesystem.c image.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

test: $(TARGET)
	./test_container.sh

init:
	mkdir -p images/hello-world examples test
