main: bin link

build: bin link
	@./primey 10000000 --file=ten-million-primes
	@zip primey-$(VER).zip ./primey ./ten-million-primes

clean:
	@rm *.o *.a *.zi *.tar test primey ten-million-primes

link: primey.o
	@$(CC) primey.o compute.o $(LDFLAGS) -o primey

bin: primey.c compute.c
	@$(CC) primey.c compute.c -c -std=$(CSTD) $(CFLAGS)

CC := gcc
CSTD = c99
OPT := 3
CFLAGS := -$(OPT) -Wall -Werror -fstack-protector -pipe -D_POSIX_SOURCE -D_GNU_SOURCE
USEMOLD := f
LDFLAGS := -flto -lm -lpthread
VER := 1.0.1

ifeq ( USEMOLD, "y" )
	LDFLAGS += -fuse-ld=mold
endif

ifeq ( FASTMATH, "y" )
	CFLAGS += -ffast-math
endif