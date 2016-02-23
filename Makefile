CC=cc
CFLAGS= -Ib64.c
CXXFLAGS=
LD=
LDFLAGS=

target = imgcat

all: imgcat.c b64.c/encode.c
	$(CC) $(CFLAGS) imgcat.c b64.c/encode.c -o $(target)

.PHONY: clean

clean:
	rm -f *.o $(target)

