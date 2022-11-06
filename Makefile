main: bin link

clean:
	@rm *.o *.a *.zip *.tar test primey

link: primey.o
	$(CC) primey.o compute.o $(LDFLAGS) -o primey

bin: primey.c compute.c
	$(CC) primey.c compute.c -c -std=$(CSTD) $(CFLAGS)

CC := gcc
CSTD = c99
CFLAGS := -O3 -Wall -Werror -fstack-protector -pipe -D_POSIX_SOURCE -D_GNU_SOURCE
USEMOLD := f
LDFLAGS := -flto -lm -lpthread

ifeq ( USEMOLD, "y" )
	LDFLAGS += -fuse-ld=mold
endif

ifeq ( FASTMATH, "y" )
	CFLAGS += -ffast-math
endif