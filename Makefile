.PHONY: bin

clean:
	@rm *.o *.a *.zip *.tar | true

bin: primey.c
	$(CC) primey.c -o primey -std=$(CSTD) $(CFLAGS)

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