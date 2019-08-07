CC=clang
CFLAGS=-Wall -Werror -O2 -std=c11 -flto
TARGETS=test1 test2 test3 test4 test5
DEPS=Bloom.o Buffer.o Murmur3.o

all: $(TARGETS)

test%: test%.c $(DEPS)
		$(CC) $(CFLAGS) -lm -o $@ $^

%.o: %.c
		$(CC) $(CFLAGS) -c $^

clean:
		$(RM) gmon.out $(TARGETS) $(DEPS)
