CC = gcc

all:
        $(CC) fnt2bmp.c -o fnt2bmp

clean:
        rm fnt2bmp

install:
        install -m 755 /usr/bin
