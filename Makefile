main: bin link

clean:
	@rm *.o *.a *.zip *.tar primey

link: primey.o
	$(CC) primey.o $(LDFLAGS) -o primey

bin: primey.c
	$(CC) primey.c -c -std=$(CSTD) $(CFLAGS)

CC := gcc
CSTD = c99
CFLAGS := -O3 -Wall -Werror -fstack-protector -pipe
USEMOLD := f
LDFLAGS := -flto -lm

ifeq ( USEMOLD, "y" )
	LDFLAGS += -fuse-ld=mold
endif

ifeq ( FASTMATH, "y" )
	CFLAGS += -ffast-math
endif