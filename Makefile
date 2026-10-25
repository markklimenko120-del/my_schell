.PHONY: build
.PHONY: clean

CC = clang
CFLAGS := -std=c99 -Wall -Wextra -Werror -g -O0 -Wpedantic -Wshadow -Wvla -Wno-non-literal-null-conversion

OBJS := build/main.o build/parsing.o build/commands.o build/logic.o

mysh: $(OBJS)
	$(CC) -o $@ $^

build/%.o: src/%.c include/config.h | build 
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build
	rm -f mysh
	rm -f include/config.h

include/config.h: config.def.h
	cp config.def.h include/config.h
