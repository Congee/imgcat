CC=cc
CFLAGS= -Ib64
CXXFLAGS=
LD=
LDFLAGS=

target = imgcat

all: imgcat.c b64/encode.c
	$(CC) $(CFLAGS) imgcat.c b64/encode.c -o $(target)

.PHONY: clean

clean:
	rm -f *.o $(target)

